(function (global) {
  'use strict';

  const PIANO_TIME_RANGE = 3000;
  const PIANO_INTERVAL_MS = 10;

  const ALT_TEXT = {
    left: '<',
    right: '>',
    up: '^',
    down: 'v',
    start: 'ST',
    select: 'SEL'
  };

  function labelForButton(name) {
    const key = name.toLowerCase();
    if (ALT_TEXT[key]) return ALT_TEXT[key];
    return key.toUpperCase();
  }

  function filterPianoEvents(events, now) {
    const cutoff = now - PIANO_TIME_RANGE;
    return events.filter((ev) => ev.endMs == null || ev.endMs > cutoff);
  }

  function computePianoBar(ev, now, pianoHeight) {
    let yRect;
    let hRect;
    if (ev.endMs == null) {
      yRect = 0;
      hRect = Math.trunc(((now - ev.startMs) * pianoHeight) / PIANO_TIME_RANGE);
    } else {
      yRect = Math.trunc(((now - ev.endMs) * pianoHeight) / PIANO_TIME_RANGE);
      hRect = Math.trunc(((ev.endMs - ev.startMs) * pianoHeight) / PIANO_TIME_RANGE);
    }
    return { x: ev.x, y: yRect, w: ev.width, h: hRect };
  }

  function createPianoRenderer(canvas, tagCanvas, pianoSkin) {
    const ctx = canvas.getContext('2d');
    const tagCtx = tagCanvas.getContext('2d');
    const eventsByButton = new Map();
    let timer = null;

    canvas.width = pianoSkin.width;
    canvas.height = pianoSkin.height;
    tagCanvas.width = pianoSkin.tag.width;
    tagCanvas.height = pianoSkin.tag.height;

    const buttonMeta = new Map();
    for (const but of pianoSkin.buttons) {
      buttonMeta.set(but.name.toLowerCase(), but);
      eventsByButton.set(but.name.toLowerCase(), []);
    }

    function drawTag() {
      const tag = pianoSkin.tag;
      tagCtx.fillStyle = tag.bgcolor;
      tagCtx.fillRect(0, 0, tagCanvas.width, tagCanvas.height);
      tagCtx.font = `${tag.fontBold ? 'bold ' : ''}${tag.fontSize}px ${tag.fontName}`;
      tagCtx.textAlign = 'center';
      tagCtx.textBaseline = 'middle';
      for (const but of pianoSkin.buttons) {
        const name = but.name.toLowerCase();
        tagCtx.fillStyle = but.color;
        tagCtx.fillText(labelForButton(name), but.x + 5, tag.height / 2);
      }
    }

    function redraw() {
      const now = Date.now();
      ctx.fillStyle = pianoSkin.bgcolor || '#000';
      ctx.fillRect(0, 0, canvas.width, canvas.height);

      for (const [name, events] of eventsByButton.entries()) {
        const meta = buttonMeta.get(name);
        if (!meta) continue;
        const filtered = filterPianoEvents(events, now);
        eventsByButton.set(name, filtered);
        for (const ev of filtered) {
          const bar = computePianoBar(
            { startMs: ev.startMs, endMs: ev.endMs, x: meta.x, width: meta.width },
            now,
            canvas.height
          );
          if (bar.h <= 0) continue;
          ctx.fillStyle = meta.color;
          ctx.fillRect(bar.x, bar.y, bar.w, bar.h);
        }
      }
    }

    function start() {
      drawTag();
      if (timer) clearInterval(timer);
      timer = setInterval(redraw, PIANO_INTERVAL_MS);
      redraw();
    }

    function stop() {
      if (timer) clearInterval(timer);
      timer = null;
    }

    function onInputChange(button, pressed, ts) {
      const name = button.toLowerCase();
      if (!eventsByButton.has(name)) return;
      const list = eventsByButton.get(name);
      if (pressed) {
        list.push({ startMs: ts, endMs: null });
      } else if (list.length > 0) {
        list[list.length - 1].endMs = ts;
      }
    }

    function setPressedFromInit(pressedNames, ts) {
      for (const name of pressedNames) {
        onInputChange(name, true, ts);
      }
    }

    return { start, stop, onInputChange, setPressedFromInit, redraw };
  }

  global.ButtonMashPiano = {
    PIANO_TIME_RANGE,
    PIANO_INTERVAL_MS,
    labelForButton,
    filterPianoEvents,
    computePianoBar,
    createPianoRenderer
  };
})(typeof window !== 'undefined' ? window : globalThis);
