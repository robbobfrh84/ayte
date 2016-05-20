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
    buildGal();
  } else { stateGal = false;
    ayteFrame.style.display = 'block';
    paletteFrame.style.display = 'block';
    liveBtn.style.display = 'inLine-block';
    gal1.style.display = 'none';
  }
}
buildGal(); //REMOVE WILL JUST ADD IT BACK TO NORMAL SWITCH WHEN GAL CLICK
function buildGal(){
  for (var i = 0; i < 64; i++){
    if (i % 8 === 0){ ayteFrame.innerHTML += '<br>';}
      createRect('gal1',10*i+2,10*i+2,10,10,'yellow',2,'blue',1,'aytep'+i);
  }
}

function createRect(container,x,y,width,height,fill,bordWidth,bordColor,opacity, recID){
  var newEl = createEl(container,'rect',[['id',recID],['stroke-width',bordWidth]
  ,['stroke',bordColor],['x',x],['y', y],['width',width],['height',height]
  ,['opacity', opacity],['fill',fill]]); return newEl;}

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
  }
  else {
    console.warn(result.operation + " failed", result);
  }
}
var getAll = {
  channel: 'gallery1',
  getall: true
}
