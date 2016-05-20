var svgElement = 'http://www.w3.org/2000/svg';
var allGalHistory;
var stateGal = false;

function swapToGal(){
  if (!stateGal){ stateGal = true;
    console.log('swapped to gallery Page');
    historyGal.flex_history(getAll, flex_history_callback);
    ayteFrame.style.display = 'none';
    paletteFrame.style.display = 'none';
    liveBtn.style.display = 'none';
    gal1.style.display = 'block';
  } else { stateGal = false;
    ayteFrame.style.display = 'block';
    paletteFrame.style.display = 'block';
    liveBtn.style.display = 'inLine-block';
    gal1.style.display = 'none';
  }
}

var w = 8; //var w=15; var gap=1.5; var rnd=3; LOOKS GOOD FOR MEDIUM THUMB
var gap = -0.75;
var rnd = 0; //remove!!!
var filter = 'url(#f1)';
function buildGal(){
  var col = -1; var row = -8;
  for (var i = 0; i < allGalHistory.count; i++){
    if (i % 8 === 0){ row+=8; col++}
      //need to build a container so that we can have click effects....
      
      buildThumbnail(i, (i-row)*((w+gap)*10), ((w+gap)*10)*col);
  }
}

function buildThumbnail(ayte, cA, rA){
  var col = 0; var row = -8;
  for (var i = 0; i < 64; i++){
    if (i % 8 === 0){ row+=8; col++}
    var c = allGalHistory.messages[ayte].message.ayte[i];
    if (c !== "" && c !== 'rgb(221, 221, 221)'){
      createRect('gal1',cA+((w+gap)*(i-row)),rA+((w+gap)*col),w,w,rnd,rnd
      ,c,0,'none',filter,'aytep'+i);
    }
  }
}

function createRect(container,x,y,width,height,rx,ry,fill,bordWidth,bordColor,filter,recID){
  var newEl = createEl(container,'rect',[['id',recID],['stroke-width',bordWidth+'px']
  ,['stroke',bordColor],['x',x+'px'],['y', y+'px'],['width',width+'px'],['height',height+'px']
  ,['filter', filter]
  ,['rx',rx],['ry',ry],['fill',fill]]); return newEl;}

//  filter="url(#f1)"

function createEl(container,type,att){
  var newObj = document.createElementNS(svgElement, type);
  for (var i=0; i<att.length; i++){ newObj.setAttributeNS(null, att[i][0],att[i][1]); }
  document.getElementById(container).appendChild(newObj); return newObj; }



// -------------------  pubnub's flex callback -------------------------------//
var historyGal = PUBNUB.init({
  publish_key: 'pub-c-a3bac365-84b3-4a6b-a8a1-67d0e2aaad3d',
  subscribe_key: 'sub-c-f0907bae-1ab6-11e6-9f24-02ee2ddab7fe'
});
historyGal.flex_history = pubnub_flex_history;
// Example of a generic callback from pubNub ...
var flex_history_callback = function(result) {
  if (!result.error) {
    console.log(result.operation + " completed", result);
    allGalHistory = result;
    buildGal();
  }
  else {
    console.warn(result.operation + " failed", result);
  }
}
var getAll = {
  channel: 'gallery1',
  getall: true
}
