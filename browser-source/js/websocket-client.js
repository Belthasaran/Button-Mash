(function () {
  'use strict';

  const IDLE_MS = 12000;
  const PONG_TIMEOUT_MS = 5000;
  const INIT_TIMEOUT_MS = 5000;
  const TICK_MS = 1000;
  const MAX_RETRY_SEC = 30;
  const COMPACT_MAIN_MAX_HEIGHT = 64;

  const view = document.body.getAttribute('data-view') || 'main';
  const root = document.getElementById('root');

  let mainRenderer = null;
  let pianoRenderer = null;
  let ws = null;
  let tickTimer = null;
  let initTimer = null;

  let connected = false;
  let lastRxAt = 0;
  let pingOutstanding = false;
  let pingDeadline = 0;
  let waitingForInit = false;
  let retryAttempt = 0;
  let retryCountdown = 0;
  let connectStatus = '';
  let inDownState = false;
  let compactOverlay = false;

  const overlayEl = document.createElement('div');
  overlayEl.id = 'connection-overlay';
  overlayEl.className = 'connection-overlay hidden';
  overlayEl.innerHTML =
    '<div class="connection-overlay-box">' +
    '<div class="connection-overlay-line1">Display Down</div>' +
    '<div class="connection-overlay-line2"></div>' +
    '</div>';
  root.appendChild(overlayEl);
  const overlayLine1 = overlayEl.querySelector('.connection-overlay-line1');
  const overlayLine2 = overlayEl.querySelector('.connection-overlay-line2');

  function wsUrl() {
    const proto = location.protocol === 'https:' ? 'wss:' : 'ws:';
    return `${proto}//${location.host}/ws?view=${encodeURIComponent(view)}`;
  }

  function retryDelaySec() {
    const attempt = Math.max(1, retryAttempt);
    return Math.min(MAX_RETRY_SEC, Math.pow(2, attempt - 1));
  }

  function updateCompactMode(mainSkin) {
    if (mainSkin) {
      compactOverlay = view === 'main'
        && mainSkin.height <= COMPACT_MAIN_MAX_HEIGHT;
    }
    overlayEl.classList.toggle('connection-overlay--compact', compactOverlay);
  }

  function updateOverlayText() {
    if (!inDownState) return;
    if (compactOverlay) {
      if (retryCountdown > 0) {
        overlayLine1.textContent = `Restart in ${retryCountdown}..`;
      } else {
        overlayLine1.textContent = 'Error, Connecting..';
      }
      overlayLine2.textContent = '';
      return;
    }
    overlayLine1.textContent = 'Display Down';
    if (retryCountdown > 0) {
      const sec = retryCountdown;
      overlayLine2.textContent = `Retart in ${sec} second${sec === 1 ? '' : 's'}`;
    } else {
      overlayLine2.textContent = `Connect: ${connectStatus || 'Connecting...'}`;
    }
  }

  function showOverlay() {
    inDownState = true;
    overlayEl.classList.remove('hidden');
    updateOverlayText();
  }

  function hideOverlay() {
    inDownState = false;
    overlayEl.classList.add('hidden');
  }

  function clearInitTimer() {
    if (initTimer !== null) {
      clearTimeout(initTimer);
      initTimer = null;
    }
  }

  function startInitTimer() {
    clearInitTimer();
    initTimer = setTimeout(() => {
      initTimer = null;
      if (!waitingForInit) return;
      connectStatus = 'Waiting for server...';
      updateOverlayText();
      failConnectAttempt();
    }, INIT_TIMEOUT_MS);
  }

  function markConnected() {
    connected = true;
    waitingForInit = false;
    pingOutstanding = false;
    pingDeadline = 0;
    retryAttempt = 0;
    retryCountdown = 0;
    connectStatus = '';
    clearInitTimer();
    hideOverlay();
    lastRxAt = Date.now();
  }

  function beginDownState() {
    if (connected) {
      connected = false;
      waitingForInit = false;
      pingOutstanding = false;
      pingDeadline = 0;
      clearInitTimer();
    }
    retryAttempt += 1;
    retryCountdown = retryDelaySec();
    connectStatus = '';
    showOverlay();
  }

  function failConnectAttempt() {
    waitingForInit = false;
    clearInitTimer();
    if (ws) {
      ws.onopen = null;
      ws.onmessage = null;
      ws.onerror = null;
      ws.onclose = null;
      try {
        ws.close();
      } catch (e) { /* ignore */ }
      ws = null;
    }
    if (!inDownState) {
      beginDownState();
    } else {
      retryAttempt += 1;
      retryCountdown = retryDelaySec();
    }
    updateOverlayText();
  }

  function handleMessage(msg) {
    lastRxAt = Date.now();
    if (msg.type === 'pong') {
      pingOutstanding = false;
      pingDeadline = 0;
      return;
    }
    if (msg.type === 'init') {
      applyInit(msg);
      markConnected();
      return;
    }
    if (msg.type === 'input') {
      applyInput(msg);
    }
  }

  function getOrCreateStack() {
    let stack = root.querySelector('.display-stack');
    if (!stack) {
      stack = document.createElement('div');
      stack.className = 'display-stack';
      root.insertBefore(stack, overlayEl);
    }
    stack.innerHTML = '';
    return stack;
  }

  function buildLayout(init) {
    if (pianoRenderer) {
      pianoRenderer.stop();
      pianoRenderer = null;
    }
    mainRenderer = null;

    const stack = getOrCreateStack();

    const showMain = view === 'main' || view === 'stacked';
    const showPiano = (view === 'piano' || view === 'stacked') && init.pianoSkin;

    if (showMain) {
      const mainCanvas = document.createElement('canvas');
      mainCanvas.className = 'main-canvas';
      stack.appendChild(mainCanvas);
      mainRenderer = ButtonMashMain.createMainRenderer(mainCanvas, init.mainSkin);
      mainRenderer.loadAssets().catch(console.error);
    }

    if (showPiano) {
      const pianoCanvas = document.createElement('canvas');
      pianoCanvas.className = 'piano-canvas';
      stack.appendChild(pianoCanvas);
      const tagCanvas = document.createElement('canvas');
      tagCanvas.className = 'tag-canvas';
      stack.appendChild(tagCanvas);
      pianoRenderer = ButtonMashPiano.createPianoRenderer(pianoCanvas, tagCanvas, init.pianoSkin);
      pianoRenderer.start();
    }
  }

  function applyInit(msg) {
    updateCompactMode(msg.mainSkin);
    buildLayout(msg);
    const ts = Date.now();
    if (mainRenderer && Array.isArray(msg.pressed))
      mainRenderer.setPressedFromInit(msg.pressed);
    if (pianoRenderer && Array.isArray(msg.pressed))
      pianoRenderer.setPressedFromInit(msg.pressed, ts);
  }

  function applyInput(msg) {
    const ts = typeof msg.ts === 'number' ? msg.ts : Date.now();
    if (!Array.isArray(msg.changes)) return;
    for (const change of msg.changes) {
      if (mainRenderer) mainRenderer.onInputChange(change.button, change.pressed);
      if (pianoRenderer) pianoRenderer.onInputChange(change.button, change.pressed, ts);
    }
  }

  function sendPing() {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    try {
      ws.send(JSON.stringify({ type: 'ping' }));
      pingOutstanding = true;
      pingDeadline = Date.now() + PONG_TIMEOUT_MS;
    } catch (e) {
      failConnectAttempt();
    }
  }

  function checkHealth() {
    if (inDownState) {
      if (retryCountdown > 0) {
        retryCountdown -= 1;
        updateOverlayText();
      }
      if (retryCountdown <= 0 && !waitingForInit && (!ws || ws.readyState === WebSocket.CLOSED)) {
        connectStatus = 'Connecting...';
        updateOverlayText();
        openSocket();
      }
      return;
    }

    if (!ws || ws.readyState !== WebSocket.OPEN) return;

    const now = Date.now();
    if (pingOutstanding && now >= pingDeadline) {
      failConnectAttempt();
      return;
    }
    if (!pingOutstanding && now - lastRxAt >= IDLE_MS)
      sendPing();
  }

  function openSocket() {
    if (ws) {
      ws.onopen = null;
      ws.onmessage = null;
      ws.onerror = null;
      ws.onclose = null;
      try {
        ws.close();
      } catch (e) { /* ignore */ }
      ws = null;
    }

    waitingForInit = true;
    connectStatus = 'Connecting...';
    updateOverlayText();

    ws = new WebSocket(wsUrl());
    ws.onopen = () => {
      lastRxAt = Date.now();
      connectStatus = 'Connecting...';
      updateOverlayText();
      startInitTimer();
    };
    ws.onmessage = (ev) => {
      let msg;
      try {
        msg = JSON.parse(ev.data);
      } catch (e) {
        return;
      }
      handleMessage(msg);
    };
    ws.onerror = () => {
      connectStatus = 'Connection failed';
      updateOverlayText();
    };
    ws.onclose = () => {
      if (connected || waitingForInit)
        failConnectAttempt();
    };
  }

  function connect() {
    if (!inDownState)
      beginDownState();
    else
      retryCountdown = retryDelaySec();
    updateOverlayText();
    if (retryCountdown <= 0)
      openSocket();
  }

  function startTick() {
    if (tickTimer !== null) return;
    tickTimer = setInterval(checkHealth, TICK_MS);
  }

  startTick();
  connect();
})();
