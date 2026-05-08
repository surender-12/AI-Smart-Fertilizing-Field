#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* ---------------- AP CONFIG ---------------- */
const char* ssid = "SmartField";
const char* password = "12345678";

ESP8266WebServer server(80);

/* ---------------- PINS ---------------- */
#define SOIL_PIN A0
#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6
#define TRIG D7
#define ECHO D8

/* ---------------- GLOBAL ---------------- */
int soilPercent = 0;
int tankLevelCM = 0;
int tankPercent = 0;

bool pumpRunning = false;
bool autoMode = true;

unsigned long pumpStartTime = 0;
unsigned long totalPumpRunTime = 0;

int fertilizerPercent = 50;

/* ---------------- FUNCTIONS ---------------- */

void stopAll() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  if (pumpRunning) {
    totalPumpRunTime += millis() - pumpStartTime;
  }
  pumpRunning = false;
}

void startWater() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  pumpRunning = true;
  pumpStartTime = millis();
}

void startFert() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  pumpRunning = true;
  pumpStartTime = millis();
}

int readTankLevel() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
  return duration * 0.034 / 2;   // Distance in cm
}

/* ---------------- AI CONTROL ---------------- */

void aiControl() {

  bool tankEmpty = tankPercent <= 5;   // Tank empty below 5%

  if (tankEmpty) {
    stopAll();
    return;
  }

  if (soilPercent < 5) {
    startWater();
    delay(1000);
    stopAll();
  }
}

/* ---------------- WEB PAGE ---------------- */

String webpage() {
  return R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Smart Field AI - Fertilizer Mixer</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: linear-gradient(135deg, #f8fafc 0%, #eef2ff 35%, #fff7ed 70%, #ecfccb 100%);
      color: #0f172a;
      min-height: 100vh;
      padding: 20px;
    }
    .container { max-width: 1200px; margin: 0 auto; }
    header { text-align: center; margin-bottom: 40px; padding: 20px; background: rgba(255,255,255,0.9); border-radius: 12px; border-left: 4px solid #10b981; }
    header h1 { font-size: 32px; margin-bottom: 5px; color: #0f172a; }
    header p { color: #475569; font-size: 14px; }
    .mode-badge { display: inline-block; padding: 8px 16px; border-radius: 20px; font-weight: 600; font-size: 12px; margin-top: 10px; text-transform: uppercase; letter-spacing: 1px; }
    .mode-badge.auto { background: #10b981; color: white; }
    .mode-badge.manual { background: #f59e0b; color: white; }
    .dashboard { display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 24px; margin-bottom: 40px; }
    .gauge-card { background: #ffffff; border: 1px solid rgba(15,23,42,0.06); border-radius: 16px; padding: 24px; text-align: center; box-shadow: 0 8px 24px rgba(15,23,42,0.06); transition: all 0.25s ease; }
    .gauge-card:hover { transform: translateY(-6px); box-shadow: 0 18px 40px rgba(15,23,42,0.08); }
    .gauge-label { color: #64748b; font-size: 13px; text-transform: uppercase; letter-spacing: 1px; margin-bottom: 12px; font-weight: 700; }
    .circular-gauge { position: relative; width: 150px; height: 150px; margin: 0 auto; }
    .gauge-svg { width: 100%; height: 100%; transform: rotate(-90deg); }
    .gauge-value { position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); text-align: center; }
    .gauge-number { font-size: 32px; font-weight: 700; color: #0f172a; line-height: 1; }
    .gauge-unit { font-size: 12px; color: #64748b; margin-top: 3px; }
    .tank-status { padding: 10px; background: #eef2ff; border-radius: 8px; font-size: 12px; color: #1e293b; margin-top: 12px; }
    .tank-status.low { background: #fff1f2; color: #7f1d1d; }
    .runtime-card { background: #f8fafc; border-left: 4px solid #3b82f6; padding: 16px; border-radius: 8px; margin-top: 12px; }
    .runtime-card h3 { color: #1e293b; font-size: 12px; text-transform: uppercase; margin-bottom: 8px; }
    .runtime-card .value { font-size: 24px; font-weight: 700; }
    .control-section { background: #ffffff; border: 1px solid rgba(15,23,42,0.06); border-radius: 16px; padding: 24px; margin-bottom: 40px; box-shadow: 0 6px 18px rgba(15,23,42,0.05); }
    .section-title { font-size: 16px; font-weight: 700; margin-bottom: 16px; color: #0f172a; text-transform: uppercase; letter-spacing: 1px; }
    .controls-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(120px, 1fr)); gap: 12px; margin-bottom: 20px; }
    button { padding: 12px 16px; border: none; border-radius: 10px; font-weight: 700; font-size: 13px; cursor: pointer; text-transform: uppercase; letter-spacing: 0.5px; transition: all 0.2s ease; display: flex; align-items: center; justify-content: center; gap: 8px; }
    .btn-primary { background: #10b981; color: white; }
    .btn-primary:disabled { opacity: 0.6; cursor: not-allowed; }
    .btn-secondary { background: #3b82f6; color: white; }
    .btn-danger { background: #ef4444; color: white; }
    .btn-warning { background: #f59e0b; color: white; }
    .fert-mixer { padding: 18px; background: #f8fafc; border: 1px solid rgba(15,23,42,0.04); border-radius: 10px; margin-top: 12px; }
    .fert-mixer-title { font-size: 14px; color: #0f172a; text-transform: uppercase; font-weight: 700; margin-bottom: 12px; }
    .mixer-input-group { display: flex; gap: 10px; margin-bottom: 12px; }
    input[type="number"] { flex: 1; padding: 10px; background: #ffffff; border: 1px solid rgba(15,23,42,0.06); border-radius: 8px; color: #0f172a; font-size: 14px; }
    input[type="number"]:focus { outline: none; border-color: #3b82f6; box-shadow: 0 6px 16px rgba(59,130,246,0.08); }
    .slider-container { margin-top: 8px; }
    .slider-label { display: flex; justify-content: space-between; margin-bottom: 8px; font-size: 12px; color: #475569; }
    input[type="range"] { width: 100%; height: 6px; border-radius: 3px; background: linear-gradient(90deg,#c7d2fe, #bbf7d0); outline: none; -webkit-appearance: none; }
    input[type="range"]::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 18px; height: 18px; border-radius: 50%; background: #0f172a; cursor: pointer; }
    .status-message { padding: 10px; background: #f1f5f9; border-left: 4px solid #10b981; border-radius: 6px; color: #0f172a; font-size: 13px; margin-top: 12px; text-align: center; min-height: 30px; }
    /* Mixing progress & spinner */
    .mix-progress-wrap { margin-top: 12px; background: #eef2ff; border-radius: 8px; overflow: hidden; height: 12px; }
    .mix-progress { height: 12px; width: 0%; background: linear-gradient(90deg,#60a5fa,#34d399); transition: width 0.2s ease; }
    .spinner { width: 36px; height: 36px; border-radius: 50%; border: 4px solid rgba(15,23,42,0.08); border-top-color: #0f172a; animation: spin 1s linear infinite; display:inline-block; vertical-align: middle; }
    @keyframes spin { to { transform: rotate(360deg); } }
    @media (max-width: 768px) { header h1 { font-size: 22px; } .dashboard { grid-template-columns: 1fr; } .controls-grid { grid-template-columns: repeat(2, 1fr); } }
  </style>
</head>
<body>
  <div class="container">
    <header>
      <h1>🌾 Smart Field AI</h1>
      <p>Intelligent Fertilizer Mixer & Irrigation Control System</p>
      <div class="mode-badge" id="modeBadge">AUTO</div>
    </header>
    <div class="dashboard">
      <div class="gauge-card">
        <div class="gauge-label">Soil Moisture</div>
        <div class="circular-gauge">
          <svg class="gauge-svg" viewBox="0 0 100 100">
            <circle cx="50" cy="50" r="45" fill="none" stroke="#eef2ff" stroke-width="6"/>
            <circle id="soilProgress" cx="50" cy="50" r="45" fill="none" stroke="#10b981" stroke-width="6" stroke-dasharray="0, 282.7" stroke-linecap="round" style="transition: stroke-dasharray 0.35s ease;"/>
          </svg>
          <div class="gauge-value"><div class="gauge-number" id="soilValue">-</div><div class="gauge-unit">%</div></div>
        </div>
        <div class="tank-status" id="soilStatus">Checking...</div>
      </div>
      <div class="gauge-card">
        <div class="gauge-label">Tank Level</div>
        <div class="circular-gauge">
          <svg class="gauge-svg" viewBox="0 0 100 100">
            <circle cx="50" cy="50" r="45" fill="none" stroke="#f1f5f9" stroke-width="6"/>
            <circle id="tankProgress" cx="50" cy="50" r="45" fill="none" stroke="#3b82f6" stroke-width="6" stroke-dasharray="0, 282.7" stroke-linecap="round" style="transition: stroke-dasharray 0.35s ease;"/>
          </svg>
          <div class="gauge-value"><div class="gauge-number" id="tankValue">-</div><div class="gauge-unit">%</div></div>
        </div>
        <div class="tank-status" id="tankStatus">Checking...</div>
      </div>
      <div class="gauge-card">
        <div class="gauge-label">Pump Runtime</div>
        <div class="runtime-card"><h3>Total Running Time</h3><div class="value" id="runtimeValue">0:00</div></div>
        <div class="tank-status" style="background:#fff7ed;color:#92400e;border-color:#92400e;">Session Duration Tracker</div>
      </div>
    </div>
    <div class="control-section">
      <div class="section-title">Quick Controls</div>
      <div class="controls-grid">
        <button id="btnWater" class="btn-primary" onclick="fetch('/water')">💧 Water Only</button>
        <button id="btnFert100" class="btn-warning" onclick="fetch('/fert100')">⚡ Full Strength</button>
        <button id="btnAuto" class="btn-secondary" onclick="fetch('/auto')">🤖 Auto Mode</button>
        <button id="btnStop" class="btn-danger" onclick="fetch('/stop')">⛔ STOP</button>
      </div>
      <div class="fert-mixer">
        <div class="fert-mixer-title">🧪 Fertilizer Mixer Control</div>
        <div class="slider-container">
          <div class="slider-label"><span>Fertilizer Concentration</span><span id="fertDisplay">50%</span></div>
          <input type="range" id="fertSlider" min="1" max="100" value="50" style="cursor: pointer;">
        </div>
        <div class="mixer-input-group" style="margin-top: 12px;"><input type="number" id="fertInput" min="1" max="100" value="50" placeholder="Enter %"><button id="btnApply" class="btn-secondary" onclick="setFert()" style="flex: 0 0 auto;">Apply Mix</button></div>
        <div class="status-message" id="statusMessage">Ready to mix</div>
        <div class="mix-progress-wrap" style="display:none;" id="mixWrap"><div id="mixProgress" class="mix-progress"></div></div>
        <div id="mixSpinner" style="display:none;margin-top:12px;text-align:center;"><span class="spinner"></span></div>
      </div>
    </div>
  </div>
  <script>
    const circumference = 282.7;
    function updateGuage(element, value) { const offset = circumference - (value / 100) * circumference; element.style.strokeDasharray = `${circumference - offset}, ${circumference}`; }
    function updateData() {
      fetch('/data').then(res => res.json()).then(data => {
        const soil = data.soil || 0; const tank = data.tank || 0; const runtime = parseInt(data.runtime) || 0; const mode = data.mode || "AUTO";
        document.getElementById('soilValue').innerText = soil; document.getElementById('tankValue').innerText = tank; document.getElementById('modeBadge').innerText = mode; document.getElementById('modeBadge').className = 'mode-badge ' + (mode === 'AUTO' ? 'auto' : 'manual');
        updateGuage(document.getElementById('soilProgress'), soil); updateGuage(document.getElementById('tankProgress'), tank);
        if (soil < 30) { document.getElementById('soilStatus').innerText = '⚠️ Dry - Water Needed'; document.getElementById('soilStatus').className = 'tank-status low'; } else if (soil < 60) { document.getElementById('soilStatus').innerText = '✓ Optimal Range'; document.getElementById('soilStatus').className = 'tank-status'; } else { document.getElementById('soilStatus').innerText = '💧 Moisture High'; document.getElementById('soilStatus').className = 'tank-status'; }
        if (tank < 30) { document.getElementById('tankStatus').innerText = '🚨 Tank Low'; document.getElementById('tankStatus').className = 'tank-status low'; } else if (tank < 60) { document.getElementById('tankStatus').innerText = '⚠️ Refill Soon'; document.getElementById('tankStatus').className = 'tank-status'; } else { document.getElementById('tankStatus').innerText = '✅ Tank Full'; document.getElementById('tankStatus').className = 'tank-status'; }
        const minutes = Math.floor(runtime / 60); const seconds = runtime % 60; document.getElementById('runtimeValue').innerText = minutes + ':' + (seconds < 10 ? '0' : '') + seconds;
      }).catch(()=>{});
    }
    function setControlsEnabled(enabled){ document.querySelectorAll('.controls-grid button, #fertSlider, #fertInput, #btnApply').forEach(el=>{ el.disabled = !enabled; if(!enabled) el.classList && el.classList.add('disabled'); else el.classList && el.classList.remove('disabled'); }); }
    function setFert(){ const p = Number(document.getElementById('fertInput').value); if (!p || p < 1 || p > 100){ document.getElementById('statusMessage').innerText = '❌ Enter value 1-100'; return; } document.getElementById('statusMessage').innerText = '⏳ Mixing ' + p + '%...'; setControlsEnabled(false); document.getElementById('mixWrap').style.display = 'block'; document.getElementById('mixSpinner').style.display = 'block'; document.getElementById('mixProgress').style.width = '0%';
      let prog = 0; const interval = setInterval(()=>{ prog = Math.min(95, prog + Math.round(Math.random()*8)+2); document.getElementById('mixProgress').style.width = prog + '%'; }, 300);
      fetch('/fert?p=' + p).then(()=>{ clearInterval(interval); document.getElementById('mixProgress').style.width = '100%'; document.getElementById('statusMessage').innerText = '✅ Mix applied: ' + p + '%'; setTimeout(()=>{ document.getElementById('mixWrap').style.display = 'none'; document.getElementById('mixSpinner').style.display = 'none'; document.getElementById('mixProgress').style.width = '0%'; document.getElementById('statusMessage').innerText = 'Ready to mix'; setControlsEnabled(true); }, 1200); }).catch(()=>{ clearInterval(interval); document.getElementById('statusMessage').innerText = '❌ Error mixing'; document.getElementById('mixSpinner').style.display = 'none'; setControlsEnabled(true); }); }
    document.getElementById('fertSlider').addEventListener('input', function(){ const val = this.value; document.getElementById('fertInput').value = val; document.getElementById('fertDisplay').innerText = val + '%'; });
    document.getElementById('fertInput').addEventListener('input', function(){ const val = Math.max(1, Math.min(100, this.value)); document.getElementById('fertSlider').value = val; document.getElementById('fertDisplay').innerText = val + '%'; });
    updateData(); setInterval(updateData, 2000);
  </script>
</body>
</html>
)=====";
}

/* ---------------- ROUTES ---------------- */

void handleData() {

  unsigned long runtimeSec = totalPumpRunTime / 1000;

  String json = "{";
  json += "\"soil\":" + String(soilPercent) + ",";
  json += "\"tank\":" + String(tankPercent) + ",";
  json += "\"runtime\":" + String(runtimeSec) + ",";
  json += "\"mode\":\"" + String(autoMode ? "AUTO" : "MANUAL") + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleWater() {
  autoMode = false;
  stopAll();
  startWater();
  delay(3000);
  stopAll();
  server.send(200, "text/plain", "Water Burst");
}

void handleFert100() {
  autoMode = false;
  stopAll();
  startFert();
  delay(3000);
  stopAll();
  server.send(200, "text/plain", "Fertilizer Burst");
}

void handleStop() {
  autoMode = false;
  stopAll();
  server.send(200, "text/plain", "Stopped");
}

void handleAuto() {
  autoMode = true;
  stopAll();
  server.send(200, "text/plain", "Auto Mode Enabled");
}

/* ---------------- SETUP ---------------- */

void setup() {

  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  stopAll();

  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());

  server.on("/", [](){ server.send(200,"text/html",webpage()); });
  server.on("/data", handleData);
  server.on("/water", handleWater);
  server.on("/fert100", handleFert100);
  server.on("/stop", handleStop);
  server.on("/auto", handleAuto);

  server.begin();
}

/* ---------------- LOOP ---------------- */

void loop() {

  server.handleClient();

  soilPercent = map(analogRead(SOIL_PIN),1023,0,0,100);
  soilPercent = constrain(soilPercent,0,100);

  // Tank height = 10 cm
  tankLevelCM = readTankLevel();
  tankPercent = map(tankLevelCM,10,0,0,100);
  tankPercent = constrain(tankPercent,0,100);

  if(autoMode){
    aiControl();
  }

  delay(1000);
}