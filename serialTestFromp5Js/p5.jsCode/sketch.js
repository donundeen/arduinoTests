let port;

function setup() {
  createCanvas(400, 400);
  port = createSerial();
  let usedPorts = usedSerialPorts();
  if (usedPorts.length > 0) {
    port.open(usedPorts[0], 9600);
  }  

}

function draw() {
  background(220);
  if (port.opened()) {
    let str = port.readUntil("\n");
    if(str !== ""){
      console.log("got", str);
    }else{
     // console.log("no ", str);
    }
  }
}

function mouseMoved(){
//  console.log(mouseX);
  let timing = mouseX;// * 10;
  console.log("sending", mouseX)
  port.write(String(timing)+"\n");
}

function connectSerial(){
  port.open(9600);

}