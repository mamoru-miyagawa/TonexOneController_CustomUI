// Shared data, palette & primitives for the ToneX Controller redesign.

const { useState, useEffect, useRef, useMemo } = React;

// ─── EFFECT CATALOG ─────────────────────────────────────────────────────────
// Mirrors the original eez-project tabs:
//   gate / comp / eq / amp / cab / mod / dly / rvb
// `toggles` = which switches the effect actually has on the device.
//   enable  — master on/off  (most effects)
//   post    — placement after amp instead of pre  (most)
//   sync    — quantize rate to BPM  (mod, dly)
//   ping    — ping-pong stereo  (dly)
//   cabByp  — bypass the cabinet IR  (cab)
// `models` = real device choices (no inventions). Each model carries a
//   short abbrev shown on the chain chip and a glyph used when active.
const EFFECTS = [
  {
    id: "gate",
    name: "Noise Gate",
    short: "GATE",
    color: "#F26B5E",
    glyph: "⎍",
    toggles: ["enable", "post"],
    params: [
      { id: "thr",  label: "Threshold", unit: "dB",  min: -90, max: 0,   def: -52 },
      { id: "rel",  label: "Release",   unit: "ms",  min: 1,   max: 500, def: 120 },
      { id: "dpt",  label: "Depth",     unit: "dB",  min: 0,   max: 60,  def: 40 },
    ],
  },
  {
    id: "comp",
    name: "Compressor",
    short: "COMP",
    color: "#4FB3D9",
    glyph: "◈",
    toggles: ["enable", "post"],
    params: [
      { id: "thr",  label: "Threshold", unit: "dB",  min: -60, max: 0,   def: -18 },
      { id: "atk",  label: "Attack",    unit: "ms",  min: 0,   max: 100, def: 12 },
      { id: "gn",   label: "Gain",      unit: "dB",  min: 0,   max: 24,  def: 6 },
    ],
  },
  {
    id: "eq",
    name: "EQ",
    short: "EQ",
    color: "#B388F2",
    glyph: "≡",
    toggles: ["post"],
    params: [
      { id: "bs",   label: "Bass",   unit: "",  min: -12, max: 12, def: 2 },
      { id: "mid",  label: "Mid",    unit: "",  min: -12, max: 12, def: -3 },
      { id: "mq",   label: "Mid Q",  unit: "",  min: 0,   max: 10, def: 4 },
      { id: "tr",   label: "Treble", unit: "",  min: -12, max: 12, def: 4 },
    ],
  },
  {
    id: "amp",
    name: "Amp",
    short: "AMP",
    color: "#E8B547",
    glyph: "▤",
    toggles: ["enable"],
    cabIRs: [
      "Default", "4x12 V30", "2x12 Greenback", "1x12 Tweed",
      "Open-back 1x12", "User IR 01", "User IR 02",
    ],
    params: [
      { id: "gn",  label: "Gain",     unit: "",  min: 0, max: 10, def: 6.5 },
      { id: "vol", label: "Volume",   unit: "",  min: 0, max: 10, def: 5.0 },
      { id: "prs", label: "Presence", unit: "",  min: 0, max: 10, def: 4.5 },
      { id: "dpt", label: "Depth",    unit: "",  min: 0, max: 10, def: 5.2 },
    ],
  },
  {
    id: "cab",
    name: "Cabinet",
    short: "CAB",
    color: "#D98452",
    glyph: "▥",
    toggles: ["cabByp"],
    cabIRs: [
      "Default", "4x12 V30", "2x12 Greenback", "1x12 Tweed",
      "Open-back 1x12", "User IR 01", "User IR 02",
    ],
    params: [],   // Cab is just an IR slot — no knobs of its own.
  },
  {
    id: "mod",
    name: "Modulation",
    short: "MOD",
    color: "#3DD9B0",
    glyph: "∿",
    toggles: ["enable", "post", "sync"],
    models: [
      { name: "Chorus",   abbrev: "CHO", glyph: "∿" },
      { name: "Flanger",  abbrev: "FLG", glyph: "≈" },
      { name: "Phaser",   abbrev: "PHS", glyph: "⌇" },
      { name: "Rotary",   abbrev: "ROT", glyph: "◐" },
      { name: "Tremolo",  abbrev: "TRM", glyph: "⊟" },
    ],
    defaultModel: 0,
    params: [
      { id: "rt",   label: "Rate",   unit: "Hz", min: 0.1, max: 10,  def: 1.2 },
      { id: "dpt",  label: "Depth",  unit: "%",  min: 0,   max: 100, def: 45 },
      { id: "mix",  label: "Mix",    unit: "%",  min: 0,   max: 100, def: 35 },
    ],
  },
  {
    id: "dly",
    name: "Delay",
    short: "DLY",
    color: "#F079C0",
    glyph: "⋯",
    toggles: ["enable", "post", "sync", "ping"],
    models: [
      { name: "Digital", abbrev: "DIG", glyph: "⋯" },
      { name: "Tape",    abbrev: "TPE", glyph: "◷" },
    ],
    defaultModel: 0,
    params: [
      { id: "tm",   label: "Time",     unit: "ms", min: 20,  max: 2000, def: 380 },
      { id: "fb",   label: "Feedback", unit: "%",  min: 0,   max: 100,  def: 38 },
      { id: "mix",  label: "Mix",      unit: "%",  min: 0,   max: 100,  def: 28 },
    ],
  },
  {
    id: "rvb",
    name: "Reverb",
    short: "REVERB",
    color: "#A186F2",
    glyph: "◊",
    toggles: ["enable", "post"],
    models: [
      { name: "Spring 1", abbrev: "SP1", glyph: "1" },
      { name: "Spring 2", abbrev: "SP2", glyph: "2" },
      { name: "Spring 3", abbrev: "SP3", glyph: "3" },
      { name: "Spring 4", abbrev: "SP4", glyph: "4" },
      { name: "Room",     abbrev: "RM",  glyph: "⌂" },
      { name: "Plate",    abbrev: "PLT", glyph: "▱" },
    ],
    defaultModel: 1,
    params: [
      { id: "mix",  label: "Mix",       unit: "%",  min: 0,   max: 100, def: 32 },
      { id: "tm",   label: "Time",      unit: "s",  min: 0.1, max: 12,  def: 2.4 },
      { id: "pre",  label: "Pre-Delay", unit: "ms", min: 0,   max: 250, def: 35 },
      { id: "col",  label: "Color",     unit: "",   min: 0,   max: 10,  def: 5.5 },
    ],
  },
];

const EFFECT_BY_ID = Object.fromEntries(EFFECTS.map(e => [e.id, e]));

// Toggle metadata — keeps labels consistent between header & chain chip.
const TOGGLE_META = {
  enable:  { label: "ON",     full: "Enable" },
  post:    { label: "POST",   full: "Post-amp" },
  sync:    { label: "SYNC",   full: "Tempo sync" },
  ping:    { label: "PING",   full: "Ping-pong" },
  cabByp:  { label: "BYP",    full: "Cab bypass" },
};

// Returns the active model object for an effect (or null).
function getModel(fx, state) {
  if (!fx.models) return null;
  const i = state?.model ?? fx.defaultModel ?? 0;
  return fx.models[clamp(i, 0, fx.models.length - 1)];
}

// ─── PRESETS ────────────────────────────────────────────────────────────────
const PRESETS = [
  {
    id: 0, bank: "A", slot: 1,
    name: "Smythbuilt OD",
    amp: "Smythbuilt 50",
    skin: "skins/smythbuilt.png",
    desc: "Mid-gain crunch built for AC/DC-style barre chords. Plate verb, no delay, gate tight.",
    bpm: 124,
    active: { gate: true,  comp: false, eq: true,  amp: true, cab: true, mod: false, dly: false, rvb: true  },
  },
  {
    id: 1, bank: "A", slot: 2,
    name: "JCM 800 Lead",
    amp: "JCM 800",
    skin: "skins/skin_jcm.png",
    desc: "Boosted JCM with a slap delay. Roll back guitar volume for clean break.",
    bpm: 132,
    active: { gate: true,  comp: false, eq: true,  amp: true, cab: true, mod: false, dly: true,  rvb: true  },
  },
  {
    id: 2, bank: "A", slot: 3,
    name: "Twin Sparkle",
    amp: "Fender Twin",
    skin: "skins/skin_fendertwin.png",
    desc: "Pristine cleans with chorus and a long hall. Single coils sing here.",
    bpm: 96,
    active: { gate: false, comp: true,  eq: true,  amp: true, cab: true, mod: true,  dly: false, rvb: true  },
  },
  {
    id: 3, bank: "A", slot: 4,
    name: "Klon Gold Push",
    amp: "Dumble ODS",
    skin: "skins/skin_jbdumble1.png",
    desc: "Always-on Klon-style boost into a Dumble for that overdriven liquid lead tone.",
    bpm: 110,
    active: { gate: true,  comp: false, eq: true,  amp: true, cab: true, mod: false, dly: true,  rvb: true  },
  },
  {
    id: 4, bank: "B", slot: 1,
    name: "Big Muff Doom",
    amp: "Orange OR120",
    skin: "skins/skin_orangeor120.png",
    desc: "Wall-of-sound fuzz. High output, treble rolled, reverb cranked.",
    bpm: 72,
    active: { gate: true,  comp: false, eq: true,  amp: true, cab: true, mod: false, dly: false, rvb: true  },
  },
  {
    id: 5, bank: "B", slot: 2,
    name: "5150 Modern",
    amp: "Peavey 5150",
    skin: "skins/skin_5150.png",
    desc: "Tight modern high-gain. Gate clamped, scooped mids, low feedback delay.",
    bpm: 140,
    active: { gate: true,  comp: false, eq: true,  amp: true, cab: true, mod: false, dly: true,  rvb: true  },
  },
  {
    id: 6, bank: "B", slot: 3,
    name: "Plexi Brown",
    amp: "Modern Plexi",
    skin: "skins/skin_modernblackplexi.png",
    desc: "EVH-style brown sound — mid-gain plexi with light phaser swirl.",
    bpm: 118,
    active: { gate: true,  comp: false, eq: true,  amp: true, cab: true, mod: true,  dly: false, rvb: true  },
  },
  {
    id: 7, bank: "B", slot: 4,
    name: "Studio Black",
    amp: "ToneX Studio",
    skin: "skins/skin_tonexampblack.png",
    desc: "Stock IK tonal palette — transparent, neutral. Good starting point.",
    bpm: 100,
    active: { gate: false, comp: false, eq: false, amp: true, cab: true, mod: false, dly: false, rvb: false },
  },
];

// ─── SURFACE TOKENS ─────────────────────────────────────────────────────────
const SURF = {
  pedalboard: {
    bg:        "#0E0B08",
    panel:     "#1A1410",
    panelHi:   "#241C16",
    line:      "#2E251D",
    text:      "#F5EBD8",
    textDim:   "#8A7E6E",
    accent:    "#E8B547",   // amber
    accentHot: "#FFC857",
  },
};

const FONT_DISPLAY = `"Space Grotesk", "Inter", system-ui, sans-serif`;
const FONT_MONO    = `"JetBrains Mono", "SF Mono", ui-monospace, monospace`;

// ─── HELPERS ────────────────────────────────────────────────────────────────
function clamp(v, lo, hi) { return Math.min(hi, Math.max(lo, v)); }
function norm(v, lo, hi)  { return clamp((v - lo) / (hi - lo), 0, 1); }
function fmt(v, p)        { return `${Math.round(v*10)/10}`; }

// ─── BASE DEVICE FRAME ──────────────────────────────────────────────────────
function DeviceFrame({ children, surf, label }) {
  return (
    <div style={{
      width: 480, height: 320, position: "relative",
      background: surf.bg, color: surf.text,
      fontFamily: FONT_DISPLAY,
      overflow: "hidden",
      borderRadius: 4,
      boxShadow: `inset 0 0 0 1px ${surf.line}`,
    }}>
      {children}
    </div>
  );
}

// ─── STATUS PILLS (top-right of header) ─────────────────────────────────────
function StatusDot({ on, color, label }) {
  return (
    <div style={{
      display: "inline-flex", alignItems: "center", gap: 4,
      fontFamily: FONT_MONO, fontSize: 9, letterSpacing: ".12em",
      color: on ? color : "#555",
    }}>
      <span style={{
        width: 6, height: 6, borderRadius: 6,
        background: on ? color : "#333",
        boxShadow: on ? `0 0 6px ${color}` : "none",
      }}/>
      {label}
    </div>
  );
}

Object.assign(window, {
  EFFECTS, EFFECT_BY_ID, PRESETS, SURF, TOGGLE_META,
  FONT_DISPLAY, FONT_MONO,
  clamp, norm, fmt, getModel,
  DeviceFrame, StatusDot,
});
