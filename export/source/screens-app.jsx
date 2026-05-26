// Screens reference page — every screen state at 1:1, laid out for porting.
// No pan/zoom, no canvas chrome. Just a clean grid.

function Screens() {
  const groups = [
    {
      title: "Main",
      frames: [
        { label: "Main · Smythbuilt OD (A·01)", el: <PedalboardApp initialScreen="main" initialPreset={0}/> },
        { label: "Main · Twin Sparkle (A·03)", el: <PedalboardApp initialScreen="main" initialPreset={2}
            initialFxState={{
              mod: { model: 0, toggles: { enable: true } },
              rvb: { model: 4, toggles: { enable: true } },
            }}/> },
        { label: "Main · 5150 Modern (B·02) — Delay tape, ping-pong", el: <PedalboardApp initialScreen="main" initialPreset={5}
            initialFxState={{
              dly: { model: 1, toggles: { enable: true, sync: true, ping: true } },
              rvb: { model: 5, toggles: { enable: true } },
            }}/> },
      ],
    },
    {
      title: "Preset browser",
      frames: [
        { label: "Browser · default scroll position", el: <PedalboardApp initialScreen="browse" initialPreset={0}/> },
      ],
    },
    {
      title: "Effect drill-in",
      frames: [
        { label: "Gate · enable + post", el: <PedalboardApp initialScreen="effect" initialEffect="gate"
            initialFxState={{ gate: { toggles: { enable: true, post: false } } }}/> },
        { label: "Compressor", el: <PedalboardApp initialScreen="effect" initialEffect="comp"
            initialFxState={{ comp: { toggles: { enable: true } } }}/> },
        { label: "EQ · post-only", el: <PedalboardApp initialScreen="effect" initialEffect="eq"
            initialFxState={{ eq: { toggles: { post: true } } }}/> },
        { label: "Amp · Cab IR slot", el: <PedalboardApp initialScreen="effect" initialEffect="amp"
            initialFxState={{ amp: { cabIR: 1, toggles: { enable: true } } }}/> },
        { label: "Cabinet · IR-only, no knobs", el: <PedalboardApp initialScreen="effect" initialEffect="cab"
            initialFxState={{ cab: { cabIR: 2, toggles: { cabByp: false } } }}/> },
        { label: "Modulation · Phaser, post + sync", el: <PedalboardApp initialScreen="effect" initialEffect="mod"
            initialFxState={{ mod: { model: 2, toggles: { enable: true, post: true, sync: true } } }}/> },
        { label: "Delay · Tape, all 3 modifiers", el: <PedalboardApp initialScreen="effect" initialEffect="dly"
            initialFxState={{ dly: { model: 1, toggles: { enable: true, post: true, sync: true, ping: true } } }}/> },
        { label: "Reverb · Plate", el: <PedalboardApp initialScreen="effect" initialEffect="rvb"
            initialFxState={{ rvb: { model: 5, toggles: { enable: true } } }}/> },
      ],
    },
  ];

  return (
    <div style={{
      padding: 32, maxWidth: 1100, margin: "0 auto",
      fontFamily: '"Space Grotesk", system-ui, sans-serif', color: "#F5EBD8",
    }}>
      <h1 style={{
        fontSize: 28, fontWeight: 700, margin: "0 0 4px",
        letterSpacing: "-0.02em",
      }}>ToneX Controller · Screens</h1>
      <div style={{
        fontSize: 13, color: "#8A7E6E", marginBottom: 32,
        fontFamily: '"JetBrains Mono", monospace', letterSpacing: ".05em",
      }}>480×320 · pedalboard direction · every screen at 1:1 device pixels</div>

      {groups.map(g => (
        <section key={g.title} style={{ marginBottom: 40 }}>
          <h2 style={{
            fontSize: 11, fontWeight: 700, margin: "0 0 12px",
            color: "#E8B547", letterSpacing: ".3em",
            fontFamily: '"JetBrains Mono", monospace',
          }}>{g.title.toUpperCase()}</h2>
          <div style={{
            display: "grid",
            gridTemplateColumns: "repeat(2, 480px)",
            gap: 24,
          }}>
            {g.frames.map((f, i) => (
              <div key={i}>
                <div style={{
                  fontSize: 10, color: "#8A7E6E", marginBottom: 6,
                  fontFamily: '"JetBrains Mono", monospace', letterSpacing: ".05em",
                }}>{f.label}</div>
                <div style={{
                  width: 480, height: 320,
                  boxShadow: "0 8px 24px rgba(0,0,0,0.4)",
                }}>{f.el}</div>
              </div>
            ))}
          </div>
        </section>
      ))}
    </div>
  );
}

ReactDOM.createRoot(document.getElementById("root")).render(<Screens/>);
