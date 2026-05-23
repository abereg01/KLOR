# KLOR — ZMK Firmware Config

Personal ZMK firmware configuration for the **KLOR** split ergonomic keyboard.

![Keymap Overview](draw/base.svg)

---

## Hardware

| Component | Choice |
|-----------|--------|
| Keyboard  | KLOR (Saegewerk) — 42-key column-stagger split |
| MCU       | nice!nano v2 (both halves) |
| Connection | TRRS cable (wired split) |
| Encoders  | Rotary encoder on each half |
| Dongle    | Seeed XIAO BLE (BLE central + USB HID to host) |
| Display   | Prospector "Saturn" — ST7789V 240×280 IPS, mounted on the dongle |
| Firmware  | [ZMK](https://zmk.dev) (Zephyr 4.1, ZMK main) |

### Topology

Both halves are BLE peripherals; the XIAO BLE dongle is the split central and the only device that speaks HID over USB to the host. The 1.69" Saturn display sits on the dongle. See `klor_dongle_topology.md` for the rationale.

---

## Layers

| # | Name    | Activated by |
|---|---------|-------------|
| 0 | BASE    | Default |
| 1 | COLEMAK | Toggle via `td_colemak` (left inner thumb) |
| 2 | SYSTEM  | Toggle/hold via `mo_tog SYSTEM` (left outer thumb) |
| 3 | RAISE   | Hold either `SPC/RAISE` thumb |
| 4 | NAV     | Single hold of right outer thumb (`NAV/NUM`) |
| 5 | NUM     | Double-tap right outer thumb → Smart-Num |
| 6 | SCROLL  | Hold right encoder button |

---

## Key Features

### Homerow Mods
Tap for the letter, hold for a modifier — no extra keys needed.

| Key | Tap | Hold |
|-----|-----|------|
| A   | A   | GUI  |
| S   | S   | Alt  |
| D   | D   | Shift |
| F   | F   | Ctrl |
| J   | J   | GUI  |
| K   | K   | Shift |
| L   | L   | Alt  |
| ;   | ;   | Ctrl |

### Mod-Morphs
Two characters on one key, split by whether Shift is held.

| Key | Tap | Shift |
|-----|-----|-------|
| `,` | `,` | `;`   |
| `.` | `.` | `:`   |
| `/` | `?` | `!`   |
| Right-space | Space | Caps Word |

> Forward slash `/` is still reachable via the **K + ,** combo.

### Tap Dances

| Key | Single tap | Double tap |
|-----|-----------|------------|
| `A` | `A` / hold → GUI | `Ä` |
| `S+X` combo | `'` | `` ` `` |
| NAV/NUM thumb | Hold → NAV layer | Double-tap → Smart-Num |

### Smart-Num (auto-layer)
Double-tapping the NAV/NUM thumb activates the NUM layer via [urob's zmk-auto-layer](https://github.com/urob/zmk-auto-layer). The layer stays active as long as you type numbers or any of `BSPC DEL . , + - * / =`. It deactivates automatically on any other keypress — space, enter, letters, `?`, `!` all close it. No manual exit needed.

### Caps Word
Hold Shift and tap the right thumb space key to activate Caps Word. Types in ALL CAPS and auto-deactivates on any key outside `A–Z`, `_`, `BSPC`, or `DEL`.

### Encoders

| Encoder | Rotate | While holding button |
|---------|--------|----------------------|
| Left    | Scroll up / down | — |
| Right   | Scroll up / down | Scroll left / right |

Horizontal scroll uses ZMK's pointing layer (`msc SCRL_LEFT/RIGHT`) for reliable cross-app support.

### Combos
All combos use a 50 ms timeout with a 100 ms prior-idle guard. See `combos.dtsi` for the full position map.

**Navigation / editing**
- `W + E` → Escape  &ensp; `S + D` → Tab
- `R + T` → Delete  &ensp; `Y + I` → Delete
- `F + G` → Backspace  &ensp; `H + J` → Backspace

**Clipboard**
- `X + C` → Copy &ensp; `C + V` → Paste &ensp; `X + V` → Cut

**Symbols — left side**
- `W+S` → `@` &ensp; `E+D` → `#` &ensp; `R+F` → `$` &ensp; `T+G` → `%`
- `S+X` → `'`/`` ` `` &ensp; `D+C` → `\` &ensp; `F+V` → `=` &ensp; `G+B` → `~`

**Symbols — right side**
- `Y+H` → `^` &ensp; `I+J` → `+` &ensp; `O+K` → `*` &ensp; `P+L` → `&`
- `H+N` → `_` &ensp; `J+M` → `-` &ensp; `K+,` → `/` &ensp; `L+.` → `|`

**Brackets**
- Top row: `I+O` → `(` &ensp; `O+P` → `)`
- Home row: `J+K` → `(` &ensp; `K+L` → `)`
- Bottom row: `M+,` → `[` &ensp; `,+.` → `]`

**Special characters**
- `A + S` → `Å`

---

## Dongle Display (Saturn screen)

The XIAO BLE dongle drives a 1.69" ST7789V IPS panel rotated to a **280 × 240** landscape LVGL canvas. The display module formerly lived in the standalone [`prospector`](https://github.com/carrefinho/prospector) repo and was absorbed into this repo on **2026-05-23** — the shield is now `klor_dongle_screen` and lives entirely under `config/boards/shields/klor_dongle_screen/`. There is no external module dependency; `west.yml` only pulls ZMK and `zmk-auto-layer`.

### Layouts

The dongle screen ships four mutually-exclusive layouts, selectable via `CONFIG_PROSPECTOR_STATUS_SCREEN_*`:

| Layout    | Description |
|-----------|-------------|
| `CLASSIC` | Upstream Prospector layout — modifier row, layer name, battery rings |
| `RADII`   | Colorful grid layout with rounded widgets |
| `FIELD`   | Minimal layout with flowing typing-activity animation (line segments react to WPM) |
| `OPERATOR` | **Default for this build** — technical, information-dense KLOR look (modifier row, WPM meter, layer dots, split-connection bars, KLOR・スクービー wordmark) |

The active layout is set with the `PROSPECTOR_STATUS_SCREEN_LAYOUT` choice in `Kconfig`. The default for KLOR is `OPERATOR`.

### Operator layout — at a glance

```
0                                                  280
0 ┌──────────────────────────────────────────────────┐
  │  ⌘ ⌥ ⌃ ⇧                                         │ ← modifier row
  │  ▬ ▬ ▬ ▬ ▬ ▬ ▬ ▬ ▬ ▬ ▬ ▬ ▬ ▬ ▬   12  BASE        │ ← WPM meter + layer name
  │  ●  ○  ○  ○  ○  ○  ○                             │ ← layer dots
  │                  KLOR・スクービー                │
  │  ▬▬▬▬▬▬▬▬▬▬▬▬   ▬▬▬▬▬▬▬▬▬▬▬▬                    │ ← split-connection bars
240 └──────────────────────────────────────────────────┘
```

### Widgets

| Widget | Purpose |
|--------|---------|
| Modifier indicator | Live `⌘ ⌥ ⌃ ⇧` row; active mods take per-modifier accent colors (green/yellow/purple/blue). Caps Word lights all four in amber. |
| WPM meter | 15-bar histogram with a held peak indicator and a numeric WPM readout. Smoothed (fast attack, slow decay). |
| Layer display | Layer name (uppercase) next to the WPM meter, plus a row of layer dots. Active dot is Nord4 snow. |
| Split-connection bars | Two 4px bars near the bottom — Nord3 grey when a half is disconnected, Nord8 blue when connected. |
| Wordmark | `KLOR・スクービー`, three-color: blue / snow / red. |
| Screensaver | Full-screen Scoob image swapped in after `CONFIG_ZMK_IDLE_TIMEOUT` (currently disabled; re-enable by uncommenting the headers in `status_screen.c`). |

### Layer transition behavior

The layer label retints to that layer's accent color on every `zmk_layer_state_changed` event:

| # | Name    | Accent |
|---|---------|--------|
| 0 | BASE    | Nord4 snow |
| 1 | COLEMAK | Nord15 purple |
| 2 | SYSTEM  | Nord8 frost blue |
| 3 | RAISE   | Nord14 green |
| 4 | NAV     | Nord13 yellow |
| 5 | NUM     | Nord12 orange |
| 6 | SCROLL  | Nord7 frost teal |

The WPM bars are **layer-independent** — they stay Nord8 frost blue regardless of the active layer, so the meter is always easy to read at a glance. (Earlier builds tinted the bars per-layer, which made them look near-white on BASE.) Layer dots and the modifier row are also layer-independent.

### Colors

All colors live in `config/boards/shields/klor_dongle_screen/src/layouts/operator/display_colors.h` as `#define DISPLAY_COLOR_*` constants, plus two `static inline` helpers:

- `display_color_for_mod(enum modifier_type)` — per-modifier accent (GUI/Alt/Ctrl/Shift)
- `display_color_for_layer(uint8_t idx)` — per-layer accent, used by the layer label

The palette is [Nord](https://www.nordtheme.com), with the background dropped one Polar Night step below canonical Nord0 (`#212632`) because the panel's gamma makes the stock Nord0 look washed out. To recolor a widget, change the appropriate `DISPLAY_COLOR_*` define and rebuild — no other source changes needed.

### Animations & WPM tuning (FIELD layout)

The FIELD layout's flowing animation has three Kconfig knobs in `src/layouts/field/Kconfig.defconfig`:

| Option | Default | Meaning |
|--------|---------|---------|
| `PROSPECTOR_ANIMATION_WPM_REFERENCE` | 60 | WPM that the animation treats as "max speed" |
| `PROSPECTOR_ANIMATION_INTENSITY_DECAY_SEC` | 4 | Seconds of idle before line brightness fades |
| `PROSPECTOR_ANIMATION_FLOW_DECAY_SEC` | 8 | Seconds of idle before flow direction settles |

### Other display knobs

| Kconfig | Default | Effect |
|---------|---------|--------|
| `PROSPECTOR_ROTATE_DISPLAY_180` | n | Flip the canvas (mounting orientation) |
| `PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR` | y | APDS9960-driven auto-brightness |
| `PROSPECTOR_FIXED_BRIGHTNESS` | 50 | 1–100, only used when the light sensor is off |
| `PROSPECTOR_SHOW_MODIFIERS` | y | Show the `⌘ ⌥ ⌃ ⇧` row |
| `PROSPECTOR_SHOW_INACTIVE_MODIFIERS` | y | Dim inactive mods (otherwise hide them) — CLASSIC/FIELD only |
| `PROSPECTOR_MODIFIER_ORDER` | `"GACS"` | 4-char order of the modifier row |
| `PROSPECTOR_MODIFIER_OS` | `MAC` | `MAC` / `WINDOWS` / `GENERIC` — picks the glyph set |
| `PROSPECTOR_LAYER_NAME_UPPERCASE` | y | Uppercase the layer label |

Set these in `config/boards/shields/klor_dongle_screen/klor_dongle_screen.conf` (or override per-build with `-DCONFIG_…`).

> **Naming note:** the `CONFIG_PROSPECTOR_*` prefix was kept after the merge because every layout source file references these symbols by name. The shield name is KLOR-aligned (`klor_dongle_screen`) but the compile-time options retain their upstream Prospector names.

### Recent changes since the merge

- **2026-05-23** — Prospector folder absorbed into KLOR; shield renamed to `klor_dongle_screen`; CI green on `xiao_ble//zmk`.
- **2026-05-22** — Halves migrated to ZMK main + Zephyr 4.1 (HWMv2 board ID, LVGL 9). Dropped `urob` zmk fork; west.yml now pulls upstream ZMK only.
- **WPM bar color** — Active bars are now constant Nord8 frost blue (previously per-layer tinted, which read as near-white on the BASE layer).

---

## Repository Structure

```
config/
├── west.yml                                # ZMK + zmk-auto-layer (no external prospector module)
├── klor.conf                               # Board-level config flags
└── boards/shields/
    ├── klor/                               # Split halves (peripherals)
    │   ├── klor.keymap                     # Layer bindings (clean, no inline behaviors)
    │   ├── combos.dtsi                     # All combo definitions
    │   ├── macros.dtsi                     # Macro definitions
    │   ├── behaviors.dtsi                  # Homerow-mod hold-tap
    │   ├── tapdance.dtsi                   # Tap-dance behaviors
    │   └── morphs.dtsi                     # Mod-morphs + scroll encoder
    ├── klor_dongle/                        # Dongle keyless-input stubs (central)
    └── klor_dongle_screen/                 # Saturn display shield (absorbed from prospector)
        ├── klor_dongle_screen.conf         # Display Kconfig overrides
        ├── klor_dongle_screen.overlay      # Display + light-sensor DT bindings
        ├── Kconfig{.shield,.defconfig}     # Layout choice + LVGL tuning
        └── src/
            ├── custom_status_screen.c      # Dispatches to the chosen layout
            ├── display_rotate_init.c       # 270° rotate → 280×240 landscape canvas
            ├── brightness.c                # APDS9960 auto-brightness loop
            ├── modifier_order.c            # GACS ordering + OS glyph pick
            └── layouts/
                ├── classic/                # Upstream Prospector layout
                ├── radii/                  # Rounded-widget grid layout
                ├── field/                  # WPM-reactive flow animation
                └── operator/               # KLOR aesthetic — default
                    ├── status_screen.c     # Screen composition
                    ├── wpm_meter.{c,h}     # 15-bar histogram + peak indicator
                    ├── layer_display.{c,h} # Layer dots
                    ├── modifier_indicator.{c,h}
                    ├── split_indicator.{c,h}
                    ├── screensaver.{c,h}   # Idle Scoob (disabled)
                    ├── activity_watcher.{c,h}
                    └── display_colors.h    # Nord palette + per-layer/mod accents
draw/
├── overview.svg                            # All-layers keymap visualization
├── base.svg                                # Base layer only
└── keymap-drawer.yaml                      # keymap-drawer config
```

---

## ZMK Modules

| Module | Purpose |
|--------|---------|
| [zmk-auto-layer](https://github.com/urob/zmk-auto-layer) | Smart-Num auto-exit behavior (`num_word`) |

Modules are declared in `config/west.yml` and fetched automatically by `west update`.

---

## Building

Firmware is built automatically via **GitHub Actions** on every push. Download the `.uf2` artifacts from the **Actions** tab.

The full build matrix lives in `build.yaml`:

| Board | Shield(s) | Artifact |
|-------|-----------|----------|
| `nice_nano//zmk` | `klor_left` | Left half (peripheral) |
| `nice_nano//zmk` | `klor_right` | Right half (peripheral) |
| `xiao_ble//zmk` | `klor_dongle klor_dongle_screen` | Dongle (central + display) |
| `nice_nano//zmk` | `settings_reset` | Bond-wipe utility for halves |
| `xiao_ble//zmk` | `settings_reset` | Bond-wipe utility for dongle |

### Local build

```bash
west init -l config
west update
west build -s zmk/app -b "nice_nano//zmk" -- -DSHIELD=klor_left
west build -s zmk/app -b "nice_nano//zmk" -- -DSHIELD=klor_right
west build -s zmk/app -b "xiao_ble//zmk" -- -DSHIELD="klor_dongle klor_dongle_screen"
```

### Flashing

Double-tap reset on the target board to enter bootloader mode, then drag the matching `.uf2` onto the USB drive that appears (`NICENANO` for the halves, `XIAO-SENSE` for the dongle).

If you swap the central or otherwise need to wipe stale BLE bonds, flash the `settings_reset` artifact once, let it boot, then reflash the normal image.
