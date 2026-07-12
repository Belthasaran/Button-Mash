Button Mash is an Input Display written mainly to display SNES controller input.

It currently support SNES Classic hacked with Hakchi2 CE, Arduino based solution for real hardware (using Nintendo Spy firmware), Usb2Snes, regular controllers, and **remote UDP sources** (Button Mash protocol or RetroArch Network RetroPad).

## Remote input and mirroring

- **Change source → Remote server**: listen for Button Mash `BMIR` packets or RetroArch `remote_message` (default ports 27151 / 55400).
- **Share or Mirror Inputs** + **Mirror Targets**: forward presses to remote hosts and/or write binary / text session logs; optional **Last N** rolling CSV/TXT/JSON dashboard files (see `devdocs/BUTTONMASH_INPUT_MIRRORING_SPEC.md`).
- **OBS Browser Source** (Mirror Targets): optional localhost HTTP + WebSocket server with `/main`, `/stacked`, and `/piano` pages — see `devdocs/BROWSER_SOURCE.md`.
- **Legacy scaling (Input Display)** checkbox: per-window size compensation for OBS Window Capture on HiDPI displays.
- RetroArch does **not** natively stream local P1 while a game runs; use Remote RetroPad as a client into Button Mash, or a companion forwarder (documented in `devdocs/BUTTONMASH_RETROARCH_PROTOCOL.md`).

Build / run tests:
```bash
cd button-mash/build && qmake ../ButtonMash.pro && make
# unit tests
npm run test:buttonmash-inputformats
```

Config: set `BUTTONMASH_CONFIG=/path/to/file.conf` to use a dedicated Ini settings file (omit for the default Qt path, typically `~/.config/ButtonMash.conf`).

Debug console output (input traces, gamepad discovery, etc.) is **off** by default. Enable either:
- **Startup dialog** → check **Debug console output** (saved in settings), or
- Environment variable: `BUTTONMASH_DEBUG=1` (or `true`/`yes`/`on`) for the current session

When either is on, Button Mash and Qt gamepad framework debug logs are written to the console.

Note: if you're using a USB connection to an FxPak/Sd2Snes, the firmware will stop communicating for a period of about 5 frames, multiple times per second. This means the input display will not update during those times. If you need 100% accuracy, consider a hardware based input display solution (which Button Mash also supports)

# Skins

Regular Skin follow Nintendo Spy format so you can reuse skin written for it (see https://github.com/jaburns/NintendoSpy)

The Piano display skins format is Button Mash own, for now you need to look at the default skin provided if you want to write your own

# Licence

Button Mash project follow the GPL version 3 licence, you can find the full version of the licence on the LICENCE file.
