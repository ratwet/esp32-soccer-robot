#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>

const char* ssid     = "CUJ";
const char* password = "142482830";

WebServer server(80);

#define ENA 14
#define IN1 26
#define IN2 27
#define ENB 32
#define IN3 25
#define IN4 33

// All tunable variables (live-adjustable from controller) 
int motorSpeed = 255;  // 255=high  185=med  115=low
int turnInner  = 18;   // inner wheel % for smooth arc (4–60), steps of 2
int trimVal    = 0;    // direction balance (−20 to +20), steps of 2
                       // negative = slow left  positive = slow right

// Motor core 
inline void setMotors(bool lFwd, bool rFwd, int lSpd, int rSpd) {
  int lF = lSpd, rF = rSpd;
  // Trim: each unit = 2% speed change on that motor side
  if (trimVal < 0) lF = lSpd * (100 + trimVal * 2) / 100;
  if (trimVal > 0) rF = rSpd * (100 - trimVal * 2) / 100;
  lF = constrain(lF, 0, 255);
  rF = constrain(rF, 0, 255);
  digitalWrite(IN1, lFwd ? LOW  : HIGH);
  digitalWrite(IN2, lFwd ? HIGH : LOW);
  digitalWrite(IN3, rFwd ? LOW  : HIGH);
  digitalWrite(IN4, rFwd ? HIGH : LOW);
  ledcWrite(ENA, lF);
  ledcWrite(ENB, rF);
}

inline void stopMotors() {
  digitalWrite(IN1,LOW); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,LOW);
  ledcWrite(ENA,0); ledcWrite(ENB,0);
}

void execCmd(const String& cmd) {
  int s  = motorSpeed;
  int ti = s * turnInner / 100; // inner wheel for smooth L/R arc
  int ta = s * 45 / 100;        // inner wheel for diagonal combos

  // L/R = both motors FORWARD, inner wheel slowed → smooth arc (no spin)
  if      (cmd=="F")  setMotors(true,  true,  s,  s);
  else if (cmd=="B")  setMotors(false, false, s,  s);
  else if (cmd=="L")  setMotors(true,  true,  ti, s);
  else if (cmd=="R")  setMotors(true,  true,  s,  ti);
  else if (cmd=="FL") setMotors(true,  true,  ta, s);
  else if (cmd=="FR") setMotors(true,  true,  s,  ta);
  else if (cmd=="BL") setMotors(false, false, ta, s);
  else if (cmd=="BR") setMotors(false, false, s,  ta);
  else                stopMotors();
}

//  HTML
const char PAGE[] PROGMEM = R"html(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<meta name="apple-mobile-web-app-capable" content="yes">
<title>CUJ Robot</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}

/* Portrait block */
@media(orientation:portrait){
  .rotmsg{display:flex!important}
  .app{display:none!important}
}
.rotmsg{
  display:none;position:fixed;inset:0;background:#fff;
  z-index:9999;flex-direction:column;align-items:center;
  justify-content:center;gap:14px;font-family:Arial,sans-serif;
}
.ri{font-size:3em;animation:ro 2s ease-in-out infinite}
@keyframes ro{0%,100%{transform:rotate(0)}55%{transform:rotate(90deg)}}
.rt{font-size:.68em;letter-spacing:3px;color:#556}

/* Fullscreen overlay */
.fso{
  position:fixed;inset:0;background:#fff;z-index:999;
  display:flex;flex-direction:column;
  align-items:center;justify-content:center;
  gap:14px;font-family:Arial,sans-serif;cursor:pointer;
}
.fi{font-size:3.5em;animation:pu 1.5s ease-in-out infinite}
@keyframes pu{0%,100%{transform:scale(1)}50%{transform:scale(1.1)}}
.fh{font-size:1.1em;color:#1e2a4a;letter-spacing:4px;font-weight:900}
.fp{font-size:.6em;color:#8898bb;letter-spacing:3px}
.fb{
  background:#1e2a4a;color:#fff;border:none;
  border-radius:24px;padding:11px 30px;
  font-size:.72em;letter-spacing:2px;font-weight:bold;cursor:pointer;
}

/* Shell */
html,body{width:100vw;height:100vh;overflow:hidden}
body{background:#f2f5ff;touch-action:none;user-select:none;font-family:Arial,sans-serif}
.app{width:100vw;height:100vh;display:flex;flex-direction:column}

/* ── Top bar ── */
.tb{
  height:42px;background:#1e2a4a;flex-shrink:0;
  display:flex;align-items:center;
  justify-content:space-between;
  padding:0 10px;gap:6px;
}
.logo{color:#00c8ff;font-size:.66em;font-weight:900;letter-spacing:4px;white-space:nowrap}

/* Control group */
.cg{display:flex;align-items:center;gap:3px}
.cgl{font-size:.43em;color:#5566aa;letter-spacing:2px;margin-right:2px;white-space:nowrap}

/* Speed buttons */
.spb{
  padding:3px 7px;border-radius:4px;border:none;
  font-size:.52em;font-weight:bold;cursor:pointer;
  background:#253050;touch-action:none;
  -webkit-tap-highlight-color:transparent;transition:all .1s;
}
.spb.h{color:#ff8a9a}.spb.m{color:#ffb347}.spb.l{color:#6ddc8b}
.spb.act.h{background:#e8003a;color:#fff}
.spb.act.m{background:#e07800;color:#fff}
.spb.act.l{background:#00a844;color:#fff}

/* Adj (+/−) buttons */
.adj{
  width:20px;height:20px;border-radius:50%;border:none;
  background:#253050;color:#7788aa;font-size:.8em;font-weight:bold;
  cursor:pointer;touch-action:none;
  display:flex;align-items:center;justify-content:center;
  -webkit-tap-highlight-color:transparent;
}
.adj:active{background:#00c8ff;color:#0a1428}
.av{font-size:.58em;font-weight:bold;min-width:28px;text-align:center;color:#00c8ff;white-space:nowrap}

/* Status */
.stpill{
  background:#253050;border-radius:14px;
  padding:2px 10px;text-align:center;min-width:88px;
}
.stl{font-size:.38em;color:#445;letter-spacing:2px}
.stv{font-size:.62em;color:#00c8ff;font-weight:bold}

/* ── Gamepad area ── */
.gp{flex:1;display:flex;overflow:hidden}

/* Left half: L | R side by side */
.lp{
  width:50%;height:100%;
  display:flex;flex-direction:row;
  border-right:2px solid #c8cee8;
}

/* Right half: F top, B bottom */
.rp{
  width:50%;height:100%;
  display:flex;flex-direction:column;
}

/* All gamepad buttons */
.gb{
  flex:1;border:none;cursor:pointer;
  display:flex;flex-direction:column;
  align-items:center;justify-content:center;
  gap:6px;touch-action:none;
  -webkit-tap-highlight-color:transparent;
  transition:background .06s;
  user-select:none;
}

/* Resting backgrounds */
#dL{background:#eef3ff;border-right:1px solid #c8cee8}
#dR{background:#eef3ff}
#dF{background:#edfff5;border-bottom:1px solid #c8cee8}
#dB{background:#fff4ee}

/* Arrow icon */
.ga{
  font-size:min(5.5em,11vw);
  line-height:1;pointer-events:none;
  transition:color .06s,transform .06s;
}
#dL .ga,#dR .ga{color:#5577cc}
#dF .ga{color:#22aa55}
#dB .ga{color:#cc4411}

/* Label */
.glb{font-size:.38em;letter-spacing:3px;pointer-events:none;transition:color .06s}
#dL .glb,#dR .glb{color:#8899cc}
#dF .glb{color:#55aa77}
#dB .glb{color:#cc7755}

/* Pressed states */
#dL.on,#dR.on{background:#1833bb}
#dF.on{background:#0a7a35}
#dB.on{background:#b02808}
.gb.on .ga{color:#ffffff!important;transform:scale(.88)}
.gb.on .glb{color:rgba(255,255,255,.65)!important}
</style>
</head>
<body>

<!-- Portrait warning -->
<div class="rotmsg">
  <div class="ri">&#128241;</div>
  <p class="rt">ROTATE DEVICE TO LANDSCAPE</p>
</div>

<!-- Fullscreen tap overlay -->
<div class="fso" id="fso">
  <div class="fi">&#9917;</div>
  <h2 class="fh">CUJ ROBOT</h2>
  <p class="fp">TAP TO ENTER FULLSCREEN</p>
  <button class="fb" onclick="startFS()">TAP TO START</button>
</div>

<!-- App -->
<div class="app" id="app">

  <!-- Top bar -->
  <div class="tb">
    <div class="logo">&#9917; CUJ</div>

    <!-- Speed -->
    <div class="cg">
      <span class="cgl">SPEED</span>
      <button class="spb h act" onpointerdown="setSp('high',this)">HI</button>
      <button class="spb m"     onpointerdown="setSp('medium',this)">MED</button>
      <button class="spb l"     onpointerdown="setSp('low',this)">LO</button>
    </div>

    <!-- Turn speed -->
    <div class="cg">
      <span class="cgl">TURN</span>
      <button class="adj" onpointerdown="adjTS(-2)">&#8722;</button>
      <span class="av" id="tsv">18%</span>
      <button class="adj" onpointerdown="adjTS(2)">&#43;</button>
    </div>

    <!-- Direction balance -->
    <div class="cg">
      <span class="cgl">BALANCE</span>
      <button class="adj" onpointerdown="adjTrim(-2)">&#8722;</button>
      <span class="av" id="trimv">0</span>
      <button class="adj" onpointerdown="adjTrim(2)">&#43;</button>
    </div>

    <!-- Status -->
    <div class="stpill">
      <div class="stl">STATUS</div>
      <div class="stv" id="stv">READY</div>
    </div>
  </div>

  <!-- Gamepad -->
  <div class="gp">

    <!-- LEFT PAD: LEFT | RIGHT -->
    <div class="lp">
      <button class="gb" id="dL"
        onpointerdown="dn('L',this,event)"
        onpointerup="up('L',this,event)"
        onpointercancel="up('L',this,event)"
        onlostpointercapture="up('L',this,event)">
        <span class="ga">&#9664;</span>
        <span class="glb">LEFT</span>
      </button>
      <button class="gb" id="dR"
        onpointerdown="dn('R',this,event)"
        onpointerup="up('R',this,event)"
        onpointercancel="up('R',this,event)"
        onlostpointercapture="up('R',this,event)">
        <span class="ga">&#9654;</span>
        <span class="glb">RIGHT</span>
      </button>
    </div>

    <!-- RIGHT PAD: FORWARD | BACKWARD -->
    <div class="rp">
      <button class="gb" id="dF"
        onpointerdown="dn('F',this,event)"
        onpointerup="up('F',this,event)"
        onpointercancel="up('F',this,event)"
        onlostpointercapture="up('F',this,event)">
        <span class="ga">&#9650;</span>
        <span class="glb">FORWARD</span>
      </button>
      <button class="gb" id="dB"
        onpointerdown="dn('B',this,event)"
        onpointerup="up('B',this,event)"
        onpointercancel="up('B',this,event)"
        onlostpointercapture="up('B',this,event)">
        <span class="ga">&#9660;</span>
        <span class="glb">BACKWARD</span>
      </button>
    </div>

  </div>
</div>

<script>
// Fullscreen + landscape lock 
function startFS(){
  const el=document.documentElement;
  (el.requestFullscreen||el.webkitRequestFullscreen||function(){}).call(el);
  if(screen.orientation&&screen.orientation.lock)
    screen.orientation.lock('landscape-primary').catch(()=>{});
  document.getElementById('fso').style.display='none';
}

// Input state 
const act  = new Set();   // currently held directions
const ptrs = new Map();   // pointerId → direction key
let   last = '';          // last sent command (dedup)

const LBL = {
  F:'FORWARD',B:'BACKWARD',L:'LEFT',R:'RIGHT',
  FL:'ARC-L',FR:'ARC-R',BL:'BCK-L',BR:'BCK-R',S:'STOPPED'
};

function send(cmd){
  if(cmd===last) return;
  last=cmd;
  document.getElementById('stv').innerText=LBL[cmd]||cmd;
  fetch('/c?a='+cmd).catch(()=>{
    document.getElementById('stv').innerText='LOST!';
  });
}

function calc(){
  const f=act.has('F'),b=act.has('B'),
        l=act.has('L'),r=act.has('R');
  if(f&&l) return send('FL');
  if(f&&r) return send('FR');
  if(b&&l) return send('BL');
  if(b&&r) return send('BR');
  if(f)    return send('F');
  if(b)    return send('B');
  if(l)    return send('L');
  if(r)    return send('R');
  send('S');
}

// Pointer DOWN
function dn(k,el,ev){
  ev.preventDefault();
  el.setPointerCapture(ev.pointerId); // finger owns this element until release
  ptrs.set(ev.pointerId, k);
  el.classList.add('on');
  act.add(k);
  calc();
}

// Pointer UP
// Called by pointerup, pointercancel AND lostpointercapture
// ptrs map prevents any double-processing
function up(k,el,ev){
  if(!ptrs.has(ev.pointerId)) return; // already handled
  ptrs.delete(ev.pointerId);
  // Only release key if no OTHER finger still holds it
  if(![...ptrs.values()].includes(k)){
    el.classList.remove('on');
    act.delete(k);
    calc();
  }
}

//  Speed 
function setSp(level,btn){
  document.querySelectorAll('.spb').forEach(b=>b.classList.remove('act'));
  btn.classList.add('act');
  fetch('/s?l='+level).catch(()=>{});
}

//  Turn speed (steps of 2, range 4–60%) 
let tsCur=18;
function adjTS(d){
  tsCur=Math.max(4,Math.min(60,tsCur+d));
  document.getElementById('tsv').innerText=tsCur+'%';
  fetch('/ts?v='+tsCur).catch(()=>{});
}

//  Direction balance (steps of 2, range ±20) 
let trimCur=0;
function adjTrim(d){
  trimCur=Math.max(-20,Math.min(20,trimCur+d));
  document.getElementById('trimv').innerText=(trimCur>0?'+':'')+trimCur;
  fetch('/trim?v='+trimCur).catch(()=>{});
}
</script>
</body>
</html>
)html";

// HTTP Handlers 
void handleRoot()  { server.send_P(200,"text/html",PAGE); }

void handleCmd(){
  execCmd(server.arg("a"));
  server.send(200,"text/plain","K");
}

void handleSpeed(){
  String l=server.arg("l");
  if      (l=="high")   motorSpeed=255;
  else if (l=="medium") motorSpeed=185;
  else if (l=="low")    motorSpeed=115;
  server.send(200,"text/plain","K");
}

void handleTurnSpeed(){
  int v=server.arg("v").toInt();
  if(v>=4&&v<=60) turnInner=v;
  server.send(200,"text/plain","K");
}

void handleTrim(){
  trimVal=constrain(server.arg("v").toInt(),-20,20);
  server.send(200,"text/plain","K");
}

// Setup 
void setup(){
  Serial.begin(115200);
  setCpuFrequencyMhz(240);

  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);

  ledcAttach(ENA,8000,8);
  ledcAttach(ENB,8000,8);
  stopMotors();

  WiFi.mode(WIFI_AP);
  esp_wifi_set_ps(WIFI_PS_NONE);
  WiFi.softAP(ssid,password,6,0,4);
  WiFi.softAPConfig(
    IPAddress(192,168,4,1),
    IPAddress(192,168,4,1),
    IPAddress(255,255,255,0)
  );

  Serial.println("--------------------------");
  Serial.println("  CUJ Robot Ready!");
  Serial.print  ("  WiFi: "); Serial.println(ssid);
  Serial.println("  URL : http://192.168.4.1");
  Serial.println("--------------------------");

  server.on("/",    HTTP_GET, handleRoot);
  server.on("/c",   HTTP_GET, handleCmd);
  server.on("/s",   HTTP_GET, handleSpeed);
  server.on("/ts",  HTTP_GET, handleTurnSpeed);
  server.on("/trim",HTTP_GET, handleTrim);
  server.begin();
}

void loop(){ server.handleClient(); }
