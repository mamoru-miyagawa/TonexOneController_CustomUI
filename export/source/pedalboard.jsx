// Direction A — "Pedalboard"
// Bold, color-coded, expressive. The signal chain reads like a row of
// pedals on a board with patch cables between them.

const { useState: useStateA, useRef: useRefA } = React;

// ═══════════════════════════════════════════════════════════════════════════
//   PRIMITIVES
// ═══════════════════════════════════════════════════════════════════════════

// Big toggle (used for master Enable in header)
function PB_Toggle({ on, onToggle, color }) {
  return (
    <button onClick={onToggle} style={{
      all: "unset", cursor: "pointer",
      width: 46, height: 24, borderRadius: 12,
      background: on ? color : "#241C16",
      boxShadow: on ? `inset 0 -2px 0 rgba(0,0,0,0.3)` : `inset 0 0 0 1px #3A2F26`,
      position: "relative", transition: "background 0.15s",
    }}>
      <span style={{
        position: "absolute", left: on ? 24 : 2, top: 2,
        width: 20, height: 20, borderRadius: 10,
        background: on ? "#1A0F08" : "#5C4F40",
        transition: "left 0.15s",
      }}/>
    </button>
  );
}

// Compact pill toggle (used for modifiers: Post, Sync, Ping, Cab Byp)
function PB_PillToggle({ on, label, color, onToggle }) {
  const s = SURF.pedalboard;
  return (
    <button onClick={onToggle} style={{
      all: "unset", cursor: "pointer",
      padding: "4px 9px", borderRadius: 11,
      fontFamily: FONT_MONO, fontSize: 9.5, fontWeight: 700,
      letterSpacing: ".12em",
      background: on ? color : "transparent",
      color:      on ? "#1A0F08" : s.textDim,
      boxShadow:  on ? `0 0 8px ${color}55` : `inset 0 0 0 1px ${s.line}`,
      transition: "all 0.12s",
    }}>{label}</button>
  );
}

// Circular knob — drag HORIZONTALLY to change.
function PB_Knob({ p, val, onChange, color, onDragStart, onDragMove, onDragEnd, size = 64 }) {
  const startRef = useRefA(null);
  const t = norm(val, p.min, p.max);
  const angle = -135 + t * 270;
  const R = size / 2;
  const indicatorR = R - 7;

  const onPointerDown = (e) => {
    e.preventDefault();
    e.currentTarget.setPointerCapture(e.pointerId);
    startRef.current = { x: e.clientX, v: val };
    onDragStart?.(p, val);
  };
  const onPointerMove = (e) => {
    if (!startRef.current) return;
    const dx = e.clientX - startRef.current.x;   // right = +
    const range = p.max - p.min;
    const fine = e.shiftKey ? 4 : 1;
    const v = clamp(startRef.current.v + (dx / (220 * fine)) * range, p.min, p.max);
    onChange(v);
    onDragMove?.(v);
  };
  const onPointerUp = (e) => {
    if (!startRef.current) return;
    startRef.current = null;
    onDragEnd?.();
  };

  // Tick ring
  const ticks = [];
  for (let i = 0; i <= 10; i++) {
    const a = (-135 + (i / 10) * 270) * Math.PI / 180;
    const inner = R - 2;
    const outer = R - (i % 5 === 0 ? 5 : 3);
    ticks.push({
      x1: R + Math.cos(a) * outer,  y1: R + Math.sin(a) * outer,
      x2: R + Math.cos(a) * inner,  y2: R + Math.sin(a) * inner,
      bright: i / 10 <= t,
    });
  }
  const indA = angle * Math.PI / 180;
  const indX = R + Math.cos(indA) * indicatorR;
  const indY = R + Math.sin(indA) * indicatorR;

  return (
    <div style={{
      display: "flex", flexDirection: "column", alignItems: "center",
      gap: 2, userSelect: "none", touchAction: "none",
    }}>
      <span style={{
        fontFamily: FONT_MONO, fontSize: 9, fontWeight: 600,
        color: SURF.pedalboard.textDim, letterSpacing: ".15em",
      }}>{p.label.toUpperCase()}</span>
      <svg width={size} height={size}
        onPointerDown={onPointerDown} onPointerMove={onPointerMove}
        onPointerUp={onPointerUp} onPointerCancel={onPointerUp}
        style={{ cursor: "ew-resize", display: "block" }}>
        {ticks.map((tk, i) => (
          <line key={i} x1={tk.x1} y1={tk.y1} x2={tk.x2} y2={tk.y2}
            stroke={tk.bright ? color : "#3A2F26"} strokeWidth={tk.bright ? 1.5 : 1}
            strokeLinecap="round"
            style={tk.bright ? { filter: `drop-shadow(0 0 2px ${color})` } : null}/>
        ))}
        <circle cx={R} cy={R} r={R - 9}
          fill="#1A130D" stroke={color} strokeWidth="1" strokeOpacity="0.4"/>
        <circle cx={R} cy={R} r={R - 11} fill="url(#knobShine)"/>
        <defs>
          <radialGradient id="knobShine" cx="40%" cy="35%">
            <stop offset="0%"  stopColor="#2A1F16"/>
            <stop offset="100%" stopColor="#120C08"/>
          </radialGradient>
        </defs>
        <line x1={R} y1={R} x2={indX} y2={indY}
          stroke={color} strokeWidth="2.5" strokeLinecap="round"
          style={{ filter: `drop-shadow(0 0 3px ${color})` }}/>
        <circle cx={indX} cy={indY} r="2" fill="#FFF5D6"/>
      </svg>
      <span style={{
        fontFamily: FONT_MONO, fontSize: 11, fontWeight: 700, color: SURF.pedalboard.text,
        letterSpacing: "-0.02em", lineHeight: 1, marginTop: 1,
      }}>{fmt(val, p)}<span style={{
        marginLeft: 2, fontSize: 8, color: SURF.pedalboard.textDim, fontWeight: 400,
      }}>{p.unit}</span></span>
    </div>
  );
}

// Drag overlay that appears while a knob is being dragged.
function PB_DragOverlay({ drag }) {
  if (!drag) return null;
  const { p, val, color } = drag;
  const t = norm(val, p.min, p.max);
  return (
    <div style={{
      position: "absolute", inset: 0, zIndex: 50,
      background: "rgba(8, 5, 3, 0.92)",
      backdropFilter: "blur(2px)", WebkitBackdropFilter: "blur(2px)",
      display: "flex", flexDirection: "column", alignItems: "center",
      justifyContent: "center", pointerEvents: "none",
      animation: "pbOverlayIn 100ms ease-out",
    }}>
      <div style={{
        fontFamily: FONT_MONO, fontSize: 11, color: color,
        letterSpacing: ".25em", fontWeight: 700,
      }}>{p.label.toUpperCase()}</div>
      <div style={{
        display: "flex", alignItems: "baseline", gap: 6, marginTop: 8,
      }}>
        <span style={{
          fontFamily: FONT_DISPLAY, fontSize: 110, fontWeight: 700,
          color: "#FFF5D6", lineHeight: 0.85, letterSpacing: "-0.04em",
          textShadow: `0 0 30px ${color}99`,
          fontVariantNumeric: "tabular-nums",
        }}>{fmt(val, p)}</span>
        {p.unit && (
          <span style={{
            fontFamily: FONT_MONO, fontSize: 22, color: color,
            fontWeight: 600, letterSpacing: ".05em",
          }}>{p.unit}</span>
        )}
      </div>
      <div style={{
        marginTop: 18, width: 280, height: 4,
        background: "rgba(255,255,255,0.08)", borderRadius: 2, overflow: "hidden",
      }}>
        <div style={{
          height: "100%", width: `${t * 100}%`,
          background: color, boxShadow: `0 0 10px ${color}`,
        }}/>
      </div>
      <div style={{
        marginTop: 6, display: "flex", justifyContent: "space-between",
        width: 280, fontFamily: FONT_MONO, fontSize: 9, color: "#5C4F40",
        letterSpacing: ".1em",
      }}>
        <span>{p.min}{p.unit}</span>
        <span>{p.max}{p.unit}</span>
      </div>
    </div>
  );
}

// Selectable list popup — used for Model picker, Cab IR picker, etc.
function PB_ListPicker({ picker, onPick, onClose }) {
  if (!picker) return null;
  const s = SURF.pedalboard;
  return (
    <div onClick={onClose} style={{
      position: "absolute", inset: 0, zIndex: 60,
      background: "rgba(8, 5, 3, 0.78)",
      backdropFilter: "blur(3px)", WebkitBackdropFilter: "blur(3px)",
      display: "flex", flexDirection: "column",
      animation: "pbOverlayIn 120ms ease-out",
    }}>
      <div onClick={e => e.stopPropagation()} style={{
        margin: "auto", width: 380, maxHeight: 280,
        background: s.panel,
        borderRadius: 6,
        boxShadow: `0 8px 30px rgba(0,0,0,0.5), inset 0 0 0 1px ${picker.color}55`,
        display: "flex", flexDirection: "column",
        overflow: "hidden",
      }}>
        <div style={{
          padding: "10px 14px",
          borderBottom: `1px solid ${s.line}`,
          display: "flex", alignItems: "center", justifyContent: "space-between",
          background: `linear-gradient(180deg, ${picker.color}22 0%, transparent 100%)`,
        }}>
          <span style={{
            fontFamily: FONT_MONO, fontSize: 10, fontWeight: 700,
            color: picker.color, letterSpacing: ".2em",
          }}>{picker.title}</span>
          <button onClick={onClose} style={{
            all: "unset", cursor: "pointer",
            fontSize: 14, color: s.textDim, padding: "0 4px",
          }}>✕</button>
        </div>
        <div style={{ overflow: "auto", maxHeight: 240 }}>
          {picker.options.map((opt, i) => {
            const selected = i === picker.current;
            return (
              <button key={i} onClick={() => onPick(i)} style={{
                all: "unset", cursor: "pointer",
                display: "flex", alignItems: "center", gap: 12,
                width: "100%", boxSizing: "border-box",
                padding: "10px 14px",
                background: selected ? `${picker.color}18` : "transparent",
                borderBottom: `1px solid ${s.line}`,
                fontFamily: FONT_DISPLAY, fontSize: 14, fontWeight: selected ? 600 : 500,
                color: selected ? s.text : s.text + "CC",
              }}>
                {/* Optional glyph for the option */}
                {opt.glyph && (
                  <span style={{
                    width: 22, height: 22, borderRadius: 3,
                    background: selected ? picker.color : "#241C16",
                    display: "flex", alignItems: "center", justifyContent: "center",
                    fontFamily: FONT_DISPLAY, fontSize: 13, fontWeight: 700,
                    color: selected ? "#1A0F08" : picker.color,
                  }}>{opt.glyph}</span>
                )}
                <span style={{ flex: 1 }}>{opt.label}</span>
                {selected && (
                  <span style={{
                    fontFamily: FONT_MONO, fontSize: 10, color: picker.color,
                    letterSpacing: ".15em", fontWeight: 700,
                  }}>● ACTIVE</span>
                )}
              </button>
            );
          })}
        </div>
      </div>
    </div>
  );
}

// Field-style dropdown button — opens the list picker
function PB_DropdownButton({ label, value, glyph, color, onOpen }) {
  const s = SURF.pedalboard;
  return (
    <button onClick={onOpen} style={{
      all: "unset", cursor: "pointer",
      display: "flex", alignItems: "center", gap: 10,
      padding: "8px 12px", borderRadius: 4,
      background: "#1F1814",
      boxShadow: `inset 0 0 0 1px ${color}55`,
      flex: 1, minWidth: 0,
    }}>
      {glyph && (
        <span style={{
          width: 24, height: 24, borderRadius: 3,
          background: color,
          display: "flex", alignItems: "center", justifyContent: "center",
          fontFamily: FONT_DISPLAY, fontSize: 14, fontWeight: 700,
          color: "#1A0F08", flexShrink: 0,
        }}>{glyph}</span>
      )}
      <div style={{ flex: 1, minWidth: 0, textAlign: "left" }}>
        <div style={{
          fontFamily: FONT_MONO, fontSize: 8.5, color: s.textDim,
          letterSpacing: ".15em", fontWeight: 600, lineHeight: 1,
        }}>{label.toUpperCase()}</div>
        <div style={{
          fontFamily: FONT_DISPLAY, fontSize: 14, fontWeight: 600,
          color: s.text, lineHeight: 1.15, marginTop: 3,
          whiteSpace: "nowrap", overflow: "hidden", textOverflow: "ellipsis",
        }}>{value}</div>
      </div>
      <span style={{ color: color, fontSize: 12, opacity: 0.7 }}>▾</span>
    </button>
  );
}

// ═══════════════════════════════════════════════════════════════════════════
//   HEADER
// ═══════════════════════════════════════════════════════════════════════════
function PB_Header({ preset, onBrowse }) {
  const s = SURF.pedalboard;
  return (
    <div style={{
      height: 30, display: "flex", alignItems: "center",
      padding: "0 10px",
      borderBottom: `1px solid ${s.line}`,
      background: s.panel,
    }}>
      <button onClick={onBrowse} style={{
        all: "unset", cursor: "pointer",
        display: "flex", alignItems: "center", gap: 6,
        height: 22, padding: "0 8px",
        background: s.accent, color: "#1A1410",
        borderRadius: 3,
        fontFamily: FONT_MONO, fontSize: 11, fontWeight: 700, letterSpacing: ".05em",
      }}>{preset.bank}·{String(preset.slot).padStart(2, "0")}</button>
      <div style={{
        marginLeft: 8, fontFamily: FONT_MONO, fontSize: 9,
        color: s.textDim, letterSpacing: ".18em",
      }}>PRESET {String(preset.id + 1).padStart(2,"0")}/{PRESETS.length}</div>
      <div style={{ flex: 1 }}/>
      <div style={{ display: "flex", gap: 10, alignItems: "center" }}>
        <StatusDot on={true}  color="#4FB3D9" label="BT"/>
        <StatusDot on={false} color="#3DD9B0" label="WIFI"/>
        <StatusDot on={true}  color={s.accent} label="USB"/>
      </div>
    </div>
  );
}

// ═══════════════════════════════════════════════════════════════════════════
//   SIGNAL CHAIN
// ═══════════════════════════════════════════════════════════════════════════

// A pedal chip — shows the effect with its model glyph + abbrev when relevant.
function PB_PedalChip({ fx, active, state, onPick }) {
  const model = getModel(fx, state);
  const glyph = model?.glyph ?? fx.glyph;
  const isPost = !!state?.toggles?.post;

  return (
    <button onClick={onPick} style={{
      all: "unset", cursor: "pointer",
      width: 46, height: 84, flexShrink: 0,
      display: "flex", flexDirection: "column", alignItems: "center",
      padding: "5px 0 4px 0",
      borderRadius: 6,
      background: active ? fx.color : "#181210",
      boxShadow: active
        ? `inset 0 -3px 0 rgba(0,0,0,0.35), 0 0 14px ${fx.color}66`
        : `inset 0 0 0 1px #2A211A`,
      color: active ? "#1A0F08" : "#5C4F40",
      position: "relative",
    }}>
      {/* LED */}
      <span style={{
        width: 8, height: 8, borderRadius: 8, flexShrink: 0,
        background: active ? "#FFF5D6" : "#2A211A",
        boxShadow: active ? `0 0 8px #FFF5D6, 0 0 14px ${fx.color}` : "none",
      }}/>
      {/* POST badge — small "P" in top-right when post-routed */}
      {isPost && active && (
        <span style={{
          position: "absolute", top: 2, right: 3,
          fontFamily: FONT_MONO, fontSize: 7, fontWeight: 800,
          color: "#1A0F08",
          background: "rgba(255,245,214,0.7)", borderRadius: 2,
          padding: "1px 2px", lineHeight: 1,
        }}>P</span>
      )}
      {/* Glyph */}
      <span style={{
        fontFamily: FONT_DISPLAY, fontSize: 20, fontWeight: 700,
        lineHeight: 1, opacity: active ? 0.9 : 0.45,
        marginTop: 4,
      }}>{glyph}</span>
      {/* Short name */}
      <span style={{
        fontFamily: FONT_MONO, fontSize: 8.5, fontWeight: 700,
        letterSpacing: ".05em", marginTop: 4,
      }}>{fx.short}</span>
      {/* Model abbrev — only when active + has model */}
      {active && model && (
        <span style={{
          fontFamily: FONT_MONO, fontSize: 7, fontWeight: 600,
          letterSpacing: ".05em", marginTop: 1,
          opacity: 0.7,
        }}>{model.abbrev}</span>
      )}
    </button>
  );
}

function PB_Cable({ leftActive, rightActive }) {
  const live = leftActive && rightActive;
  const color = live ? "#E8B547" : "#2E251D";
  return (
    <svg width="10" height="84" viewBox="0 0 10 84" style={{ flexShrink: 0 }}>
      <path d="M0,42 Q5,54 10,42" stroke={color} strokeWidth="2" fill="none"
        style={{ filter: live ? "drop-shadow(0 0 3px #E8B547)" : "none" }}/>
    </svg>
  );
}

function PB_Chain({ preset, fxState, onPick }) {
  const s = SURF.pedalboard;
  return (
    <div style={{
      display: "flex", alignItems: "center", gap: 0,
      padding: "0 4px", justifyContent: "center",
    }}>
      <span style={{
        fontFamily: FONT_MONO, fontSize: 9, color: s.textDim,
        letterSpacing: ".1em", marginRight: 3, writingMode: "vertical-rl",
        transform: "rotate(180deg)", lineHeight: 1,
      }}>IN ▸</span>
      {EFFECTS.map((fx, i) => {
        const active = preset.active[fx.id];
        const prevActive = i > 0 ? preset.active[EFFECTS[i-1].id] : true;
        return (
          <React.Fragment key={fx.id}>
            {i > 0 && <PB_Cable leftActive={prevActive} rightActive={active}/>}
            <PB_PedalChip fx={fx} active={active}
              state={fxState[fx.id]}
              onPick={() => onPick(fx.id)}/>
          </React.Fragment>
        );
      })}
      <span style={{
        fontFamily: FONT_MONO, fontSize: 9, color: s.textDim,
        letterSpacing: ".1em", marginLeft: 3, writingMode: "vertical-rl",
        transform: "rotate(180deg)", lineHeight: 1,
      }}>▸ OUT</span>
    </div>
  );
}

// ═══════════════════════════════════════════════════════════════════════════
//   BPM TAP
// ═══════════════════════════════════════════════════════════════════════════
function PB_BpmTap({ bpm }) {
  const s = SURF.pedalboard;
  return (
    <div style={{
      display: "flex", flexDirection: "column", alignItems: "flex-end", gap: 2,
    }}>
      <div style={{ display: "flex", alignItems: "baseline", gap: 5 }}>
        <span style={{
          fontFamily: FONT_DISPLAY, fontSize: 26, fontWeight: 700,
          color: s.text, lineHeight: 1, letterSpacing: "-0.02em",
        }}>{bpm}</span>
        <span style={{
          fontFamily: FONT_MONO, fontSize: 9, color: s.textDim,
          letterSpacing: ".1em",
        }}>BPM</span>
      </div>
      <div style={{
        fontFamily: FONT_MONO, fontSize: 8, color: s.accent,
        letterSpacing: ".15em",
      }}>◉ TAP</div>
    </div>
  );
}

// ═══════════════════════════════════════════════════════════════════════════
//   MAIN SCREEN
// ═══════════════════════════════════════════════════════════════════════════
function PB_MainScreen({ presetIdx, setPresetIdx, fxState, onPickEffect, onBrowse }) {
  const s = SURF.pedalboard;
  const preset = PRESETS[presetIdx];
  return (
    <div style={{ width: "100%", height: "100%", display: "flex", flexDirection: "column" }}>
      <PB_Header preset={preset} onBrowse={onBrowse}/>

      {/* Hero */}
      <div style={{
        flexShrink: 0, padding: "10px 14px 8px",
        display: "flex", alignItems: "flex-start", gap: 12,
      }}>
        <div style={{ flex: 1, minWidth: 0 }}>
          <div style={{
            fontFamily: FONT_DISPLAY, fontSize: 28, fontWeight: 700,
            lineHeight: 1, color: s.text, letterSpacing: "-0.02em",
            whiteSpace: "nowrap", overflow: "hidden", textOverflow: "ellipsis",
          }}>{preset.name}</div>
          <div style={{
            marginTop: 4, fontFamily: FONT_MONO, fontSize: 10,
            color: s.textDim, letterSpacing: ".05em",
          }}>{preset.amp.toUpperCase()}</div>
        </div>
        <PB_BpmTap bpm={preset.bpm}/>
      </div>

      {/* Signal chain band */}
      <div style={{
        flexShrink: 0,
        background: `linear-gradient(180deg, ${s.panel} 0%, #0F0B07 100%)`,
        borderTop: `1px solid ${s.line}`,
        borderBottom: `1px solid ${s.line}`,
        padding: "10px 0",
      }}>
        <PB_Chain preset={preset} fxState={fxState} onPick={onPickEffect}/>
      </div>

      {/* Bottom: prev/next */}
      <div style={{
        flex: 1, display: "flex", alignItems: "center",
        padding: "0 10px", gap: 8,
      }}>
        <button onClick={() => setPresetIdx((presetIdx - 1 + PRESETS.length) % PRESETS.length)} style={{
          all: "unset", cursor: "pointer",
          flex: 1, height: 44, borderRadius: 6,
          background: s.panel, color: s.text,
          display: "flex", alignItems: "center", justifyContent: "center", gap: 8,
          boxShadow: `inset 0 0 0 1px ${s.line}`,
          fontFamily: FONT_DISPLAY, fontSize: 13, fontWeight: 600,
        }}>
          <span style={{ fontSize: 18, color: s.accent }}>◁</span>
          <span style={{ fontFamily: FONT_MONO, fontSize: 10, letterSpacing: ".1em", color: s.textDim }}>PREV</span>
        </button>
        <div style={{
          display: "flex", flexDirection: "column", alignItems: "center", gap: 1, width: 64,
        }}>
          <span style={{
            fontFamily: FONT_MONO, fontSize: 9, color: s.textDim, letterSpacing: ".1em",
          }}>SLOT</span>
          <span style={{
            fontFamily: FONT_DISPLAY, fontSize: 16, fontWeight: 700, color: s.accent, lineHeight: 1,
          }}>{preset.bank}·{String(preset.slot).padStart(2, "0")}</span>
        </div>
        <button onClick={() => setPresetIdx((presetIdx + 1) % PRESETS.length)} style={{
          all: "unset", cursor: "pointer",
          flex: 1, height: 44, borderRadius: 6,
          background: s.panel, color: s.text,
          display: "flex", alignItems: "center", justifyContent: "center", gap: 8,
          boxShadow: `inset 0 0 0 1px ${s.line}`,
          fontFamily: FONT_DISPLAY, fontSize: 13, fontWeight: 600,
        }}>
          <span style={{ fontFamily: FONT_MONO, fontSize: 10, letterSpacing: ".1em", color: s.textDim }}>NEXT</span>
          <span style={{ fontSize: 18, color: s.accent }}>▷</span>
        </button>
      </div>
    </div>
  );
}

// ═══════════════════════════════════════════════════════════════════════════
//   EFFECT DETAIL SCREEN
// ═══════════════════════════════════════════════════════════════════════════
function PB_EffectScreen({ effectId, preset, onBack, fxState, setFxState }) {
  const s = SURF.pedalboard;
  const fx = EFFECT_BY_ID[effectId];
  const state = fxState[effectId] || { toggles: {}, vals: {} };
  const [drag,   setDrag]   = useStateA(null);
  const [picker, setPicker] = useStateA(null);

  // Helper for partial state updates
  const setState = (partial) => setFxState({
    ...fxState,
    [effectId]: { ...state, ...partial },
  });
  const setParam = (pid, v)   => setState({ vals:    { ...state.vals,    [pid]: v } });
  const setToggle = (t, on)   => setState({ toggles: { ...state.toggles, [t]: on } });
  const setModel = (i)        => setState({ model: i });

  // Toggle effective state — Enable controls the master active feel
  const enable = state.toggles?.enable ?? (fx.toggles.includes("enable") ? true : null);
  const isEnabled = enable === null ? true : enable;

  const model = getModel(fx, state);
  const modifierToggles = fx.toggles.filter(t => t !== "enable");
  const hasModel = !!fx.models;
  const hasCabIR = !!fx.cabIRs;
  const cabIRIdx = state.cabIR ?? 0;

  return (
    <div style={{ width: "100%", height: "100%", display: "flex", flexDirection: "column", position: "relative" }}>
      {/* Header */}
      <div style={{
        height: 36, display: "flex", alignItems: "center",
        padding: "0 10px 0 4px", flexShrink: 0,
        background: `linear-gradient(180deg, ${fx.color}25 0%, transparent 100%)`,
        borderBottom: `1px solid ${s.line}`,
      }}>
        <button onClick={onBack} style={{
          all: "unset", cursor: "pointer",
          width: 34, height: 32, display: "flex", alignItems: "center",
          justifyContent: "center", color: s.text, fontSize: 18,
        }}>◁</button>
        <span style={{
          width: 22, height: 22, borderRadius: 4,
          background: isEnabled ? fx.color : "#241C16",
          display: "flex", alignItems: "center", justifyContent: "center",
          color: isEnabled ? "#1A0F08" : "#5C4F40", fontWeight: 700,
          fontFamily: FONT_DISPLAY, fontSize: 14, marginRight: 8,
        }}>{model?.glyph ?? fx.glyph}</span>
        <span style={{
          fontFamily: FONT_DISPLAY, fontSize: 17, fontWeight: 700,
          color: s.text, letterSpacing: "-0.01em",
        }}>{fx.name}</span>
        <span style={{
          marginLeft: 8, fontFamily: FONT_MONO, fontSize: 9,
          color: s.textDim, letterSpacing: ".15em",
        }}>{preset.bank}·{String(preset.slot).padStart(2, "0")}</span>
        <div style={{ flex: 1 }}/>
        {fx.toggles.includes("enable") && (
          <PB_Toggle on={isEnabled} onToggle={() => setToggle("enable", !isEnabled)} color={fx.color}/>
        )}
      </div>

      {/* Body */}
      <div style={{
        flex: 1, display: "flex", flexDirection: "column",
        opacity: isEnabled ? 1 : 0.4,
        padding: "8px 12px 10px",
        gap: 8,
      }}>
        {/* Modifier toggles row */}
        {modifierToggles.length > 0 && (
          <div style={{ display: "flex", gap: 6, flexShrink: 0 }}>
            {modifierToggles.map(t => (
              <PB_PillToggle key={t}
                label={TOGGLE_META[t].label}
                on={!!state.toggles?.[t]}
                color={fx.color}
                onToggle={() => setToggle(t, !state.toggles?.[t])}/>
            ))}
          </div>
        )}

        {/* Model / Cab IR dropdowns row */}
        {(hasModel || hasCabIR) && (
          <div style={{ display: "flex", gap: 8, flexShrink: 0 }}>
            {hasModel && (
              <PB_DropdownButton
                label="Model"
                value={model.name}
                glyph={model.glyph}
                color={fx.color}
                onOpen={() => setPicker({
                  title: `${fx.short} · MODEL`,
                  color: fx.color,
                  current: state.model ?? fx.defaultModel ?? 0,
                  options: fx.models.map(m => ({ label: m.name, glyph: m.glyph })),
                  onPickField: "model",
                })}/>
            )}
            {hasCabIR && (
              <PB_DropdownButton
                label="Cab IR"
                value={fx.cabIRs[cabIRIdx]}
                glyph="▥"
                color={fx.color}
                onOpen={() => setPicker({
                  title: `${fx.short} · CAB IR`,
                  color: fx.color,
                  current: cabIRIdx,
                  options: fx.cabIRs.map(name => ({ label: name })),
                  onPickField: "cabIR",
                })}/>
            )}
          </div>
        )}

        {/* Knobs row OR empty state */}
        {fx.params.length > 0 ? (
          <div style={{
            flex: 1, display: "flex", alignItems: "center", justifyContent: "space-around",
            paddingTop: 2,
          }}>
            {fx.params.map(p => {
              const v = state.vals?.[p.id] ?? p.def;
              return (
                <PB_Knob key={p.id} p={p} val={v} color={fx.color}
                  size={fx.params.length >= 4 ? 58 : 66}
                  onChange={x => setParam(p.id, x)}
                  onDragStart={(pp, vv) => setDrag({ p: pp, val: vv, color: fx.color })}
                  onDragMove ={(vv)     => setDrag(d => d && ({ ...d, val: vv }))}
                  onDragEnd  ={()       => setDrag(null)}/>
              );
            })}
          </div>
        ) : (
          <div style={{
            flex: 1, display: "flex", alignItems: "center", justifyContent: "center",
            fontFamily: FONT_MONO, fontSize: 11, color: s.textDim,
            letterSpacing: ".15em", textAlign: "center", lineHeight: 1.6,
          }}>
            {fx.id === "cab"
              ? "Cabinet IR loaded from selection above.\nUse Cab Byp to bypass the cab entirely."
              : ""}
          </div>
        )}
      </div>

      <PB_DragOverlay drag={drag}/>
      <PB_ListPicker picker={picker}
        onPick={(i) => {
          if (picker.onPickField === "model")  setModel(i);
          if (picker.onPickField === "cabIR")  setState({ cabIR: i });
          setPicker(null);
        }}
        onClose={() => setPicker(null)}/>
    </div>
  );
}

// ═══════════════════════════════════════════════════════════════════════════
//   PRESET BROWSER
// ═══════════════════════════════════════════════════════════════════════════
function PB_Browser({ presetIdx, setPresetIdx, onBack }) {
  const s = SURF.pedalboard;
  return (
    <div style={{ width: "100%", height: "100%", display: "flex", flexDirection: "column" }}>
      <div style={{
        height: 30, display: "flex", alignItems: "center",
        padding: "0 8px 0 4px",
        borderBottom: `1px solid ${s.line}`, background: s.panel,
      }}>
        <button onClick={onBack} style={{
          all: "unset", cursor: "pointer",
          width: 30, height: 28, display: "flex", alignItems: "center", justifyContent: "center",
          color: s.text, fontSize: 18,
        }}>◁</button>
        <span style={{
          fontFamily: FONT_DISPLAY, fontSize: 14, fontWeight: 700, color: s.text,
        }}>Presets</span>
        <span style={{
          marginLeft: 8, fontFamily: FONT_MONO, fontSize: 9, color: s.textDim,
          letterSpacing: ".15em",
        }}>BANK {PRESETS[presetIdx].bank}</span>
      </div>
      <div style={{ flex: 1, overflow: "auto" }}>
        {PRESETS.map((p, i) => (
          <button key={p.id} onClick={() => { setPresetIdx(i); onBack(); }} style={{
            all: "unset", cursor: "pointer",
            display: "flex", width: "100%", alignItems: "center", gap: 10,
            padding: "6px 12px 6px 8px",
            borderBottom: `1px solid ${s.line}`,
            background: i === presetIdx ? `${s.accent}15` : "transparent",
            boxSizing: "border-box",
          }}>
            <span style={{
              fontFamily: FONT_MONO, fontSize: 11, color: i === presetIdx ? s.accent : s.textDim,
              fontWeight: 700, letterSpacing: ".05em", width: 30, flexShrink: 0,
            }}>{p.bank}·{String(p.slot).padStart(2,"0")}</span>
            <div style={{
              width: 78, height: 32, flexShrink: 0,
              background: "#0A0604",
              borderRadius: 3,
              boxShadow: i === presetIdx
                ? `inset 0 0 0 1px ${s.accent}66`
                : `inset 0 0 0 1px ${s.line}`,
              backgroundImage: `url("${p.skin}")`,
              backgroundSize: "contain",
              backgroundPosition: "center",
              backgroundRepeat: "no-repeat",
            }}/>
            <div style={{ flex: 1, minWidth: 0 }}>
              <div style={{
                fontFamily: FONT_DISPLAY, fontSize: 13, fontWeight: 600,
                color: s.text, lineHeight: 1.15,
                whiteSpace: "nowrap", overflow: "hidden", textOverflow: "ellipsis",
              }}>{p.name}</div>
              <div style={{
                fontFamily: FONT_MONO, fontSize: 8.5, color: s.textDim,
                letterSpacing: ".1em", marginTop: 2,
                whiteSpace: "nowrap", overflow: "hidden", textOverflow: "ellipsis",
              }}>{p.amp.toUpperCase()} · {p.bpm} BPM</div>
            </div>
            <div style={{ display: "flex", gap: 2, flexShrink: 0 }}>
              {EFFECTS.map(fx => (
                <span key={fx.id} style={{
                  width: 4, height: 14, borderRadius: 1,
                  background: p.active[fx.id] ? fx.color : "#241C16",
                  boxShadow: p.active[fx.id] ? `0 0 4px ${fx.color}88` : "none",
                }}/>
              ))}
            </div>
          </button>
        ))}
      </div>
    </div>
  );
}

// ═══════════════════════════════════════════════════════════════════════════
//   ROOT
// ═══════════════════════════════════════════════════════════════════════════
function PedalboardApp({ initialScreen = "main", initialEffect = null, initialPreset = 0, initialFxState = {} }) {
  const [screen, setScreen]       = useStateA(initialScreen);
  const [effectId, setEffectId]   = useStateA(initialEffect);
  const [presetIdx, setPresetIdx] = useStateA(initialPreset);
  const [fxState, setFxState]     = useStateA(initialFxState);

  const pickEffect = (id) => { setEffectId(id); setScreen("effect"); };
  const back = () => setScreen("main");

  return (
    <DeviceFrame surf={SURF.pedalboard}>
      {screen === "main" && (
        <PB_MainScreen presetIdx={presetIdx} setPresetIdx={setPresetIdx}
          fxState={fxState}
          onPickEffect={pickEffect}
          onBrowse={() => setScreen("browse")}/>
      )}
      {screen === "effect" && (
        <PB_EffectScreen effectId={effectId} preset={PRESETS[presetIdx]}
          onBack={back} fxState={fxState} setFxState={setFxState}/>
      )}
      {screen === "browse" && (
        <PB_Browser presetIdx={presetIdx} setPresetIdx={setPresetIdx} onBack={back}/>
      )}
    </DeviceFrame>
  );
}

Object.assign(window, { PedalboardApp });
