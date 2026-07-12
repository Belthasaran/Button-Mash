(function (global) {
  'use strict';

  function loadImage(url) {
    return new Promise((resolve, reject) => {
      const img = new Image();
      img.onload = () => resolve(img);
      img.onerror = () => reject(new Error(`Failed to load ${url}`));
      img.src = url;
    });
  }

  function createMainRenderer(canvas, mainSkin) {
    const ctx = canvas.getContext('2d');
    canvas.width = mainSkin.width;
    canvas.height = mainSkin.height;

    const pressed = new Set();
    let background = null;
    const overlays = new Map();

    async function loadAssets() {
      if (mainSkin.background) {
        try {
          background = await loadImage(mainSkin.background);
        } catch (e) {
          console.error(e);
        }
      }
      for (const but of mainSkin.buttons) {
        const img = await loadImage(but.image);
        overlays.set(but.name.toLowerCase(), { img, but });
      }
      redraw();
    }

    function redraw() {
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      if (background) {
        ctx.drawImage(background, 0, 0, canvas.width, canvas.height);
      }
      for (const [name, entry] of overlays.entries()) {
        if (!pressed.has(name)) continue;
        const { img, but } = entry;
        ctx.drawImage(img, but.x, but.y, but.width, but.height);
      }
    }

    function onInputChange(button, isPressed) {
      const name = button.toLowerCase();
      if (!overlays.has(name)) return;
      if (isPressed) pressed.add(name);
      else pressed.delete(name);
      redraw();
    }

    function setPressedFromInit(pressedNames) {
      pressed.clear();
      for (const name of pressedNames) pressed.add(name.toLowerCase());
      redraw();
    }

    return { loadAssets, onInputChange, setPressedFromInit, redraw };
  }

  global.ButtonMashMain = { createMainRenderer, loadImage };
})(typeof window !== 'undefined' ? window : globalThis);
