// Canvas assembly — two directions, three frames each, plus a design notes header.
const { useState: useStateApp } = React;

function Note({ children }) {
  return <div style={{
    fontFamily: '"JetBrains Mono", ui-monospace, monospace',
    fontSize: 12, color: "#9A8E7A", lineHeight: 1.6,
    maxWidth: 900,
  }}>{children}</div>;
}

function CanvasRoot() {
  return (
    <DesignCanvas
      title="ToneX Controller · 480×320"
      subtitle="External display redesign — 2 directions"
      background="#0B0B0D"
    >
      <DCSection id="notes" title="Brief">
        <div style={{ padding: "8px 24px 16px", maxWidth: 940 }}>
          <Note>
            <div style={{ marginBottom: 10, color: "#E8B547" }}>
              Pedalboard · color-coded chain · per-model glyphs on chips · horizontal knob drag · live drag overlay · list-popup dropdowns · faithful Post/Sync/Ping/Cab-Byp toggles per the device.
            </div>
            <div style={{ color: "#7C7264" }}>
              Drag any knob LEFT/RIGHT to change. While dragging, the full-screen overlay shows the huge value. The model dropdown opens a real list — tap a row to pick. Modifier pills (POST · SYNC · PING) only appear on effects that have them. In the chain, an active effect with a model shows its abbreviation under the short name (e.g. MOD → CHO/FLG/PHS); a tiny <strong>P</strong> badge marks post-routed effects.
            </div>
          </Note>
        </div>
      </DCSection>

      <DCSection id="A" title="Pedalboard" subtitle="Drag a knob horizontally · tap a chip's dropdown for a list · POST/SYNC/PING toggles where the device supports them">
        <DCArtboard id="A-main" label="Main · interactive" width={480} height={320}>
          <PedalboardApp initialScreen="main" initialFxState={{
            mod: { model: 2, toggles: { enable: true, post: true } },        // Phaser, post-routed
            rvb: { model: 5, toggles: { enable: true } },                    // Plate
            dly: { model: 1, toggles: { enable: true, ping: true } },        // Tape, ping-pong
          }}/>
        </DCArtboard>
        <DCArtboard id="A-browse" label="Preset browser" width={480} height={320}>
          <PedalboardApp initialScreen="browse"/>
        </DCArtboard>
        <DCArtboard id="A-mod" label="Modulation · model picker open" width={480} height={320}>
          <PedalboardApp initialScreen="effect" initialEffect="mod" initialPreset={2}
            initialFxState={{ mod: { model: 0, toggles: { enable: true, sync: true } } }}/>
        </DCArtboard>
        <DCArtboard id="A-rvb" label="Reverb · Spring 1 selected" width={480} height={320}>
          <PedalboardApp initialScreen="effect" initialEffect="rvb" initialPreset={0}
            initialFxState={{ rvb: { model: 0, toggles: { enable: true, post: true } } }}/>
        </DCArtboard>
        <DCArtboard id="A-dly" label="Delay · Tape · Sync + Ping" width={480} height={320}>
          <PedalboardApp initialScreen="effect" initialEffect="dly" initialPreset={1}
            initialFxState={{ dly: { model: 1, toggles: { enable: true, sync: true, ping: true } } }}/>
        </DCArtboard>
        <DCArtboard id="A-amp" label="Amp · Cab IR selector" width={480} height={320}>
          <PedalboardApp initialScreen="effect" initialEffect="amp" initialPreset={0}
            initialFxState={{ amp: { cabIR: 1, toggles: { enable: true } } }}/>
        </DCArtboard>
        <DCArtboard id="A-gate" label="Gate · enable + post pill" width={480} height={320}>
          <PedalboardApp initialScreen="effect" initialEffect="gate" initialPreset={0}
            initialFxState={{ gate: { toggles: { enable: true, post: false } } }}/>
        </DCArtboard>
        <DCArtboard id="A-eq" label="EQ · post toggle only" width={480} height={320}>
          <PedalboardApp initialScreen="effect" initialEffect="eq" initialPreset={0}
            initialFxState={{ eq: { toggles: { post: true } } }}/>
        </DCArtboard>
        <DCArtboard id="A-cab" label="Cabinet · IR slot, no knobs" width={480} height={320}>
          <PedalboardApp initialScreen="effect" initialEffect="cab" initialPreset={0}
            initialFxState={{ cab: { cabIR: 2, toggles: { cabByp: false } } }}/>
        </DCArtboard>
      </DCSection>
    </DesignCanvas>
  );
}

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(<CanvasRoot/>);
