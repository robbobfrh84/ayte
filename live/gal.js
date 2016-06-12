var svgElement = 'http://www.w3.org/2000/svg';
var allGalHistory = {};
var stateGal = false;
var gCnt = 0;
var skip = [0,1,13,18,19,21,23,24,26,27,28,29,30,31,32,33,35,36,37,38,39,40,41,42,43,44,45,51,52,53
            ,54,55,56,57,59,60,61,64,66,67,69,70,72,77,78,80,81,82,83,84,85,86,87,88,89,90,91,92,93
            ,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120
            ,121,122,123,124,125,126,127,128,129,130];
//var skip = [];

function swapToGal(){
  if (!stateGal){ stateGal = true;
    if (live) {liveStatus();}
    console.log('swapped to gallery Page');
    galName.style.display = 'block';
    gal1Frame.style.display = 'block';
    ayteFrame.style.display = 'none';
    paletteFrame.style.display = 'none';
    galBtn.innerHTML = '<-Back'
    historyGal.flex_history(getAll, flex_history_callback);
  } else { stateGal = false;
    galName.style.display = 'none';
    gal1Frame.style.display = 'none';
    ayteFrame.style.display = 'block';
    paletteFrame.style.display = 'block';
    galBtn.innerHTML = '{Gallery}'
  }
}

var svgStart = '<svg xmlns="http://www.w3.org/2000/svg" width="10em" height="10em" viewBox="0 0 528 528" >';
var shaddow = '<defs><filter id="f1" height="130%" width="130%"><feGaussianBlur in="SourceAlpha" stdDeviation="5"/> <feOffset dx="5" dy="5" result="offsetblur"/><feComponentTransfer><feFuncA type="linear" slope="0.5"/></feComponentTransfer><feMerge><feMergeNode/><feMergeNode in="SourceGraphic"/></feMerge></filter></defs>';
var newgroup = '<g filter="url(#f1)" >';

function buildGal(){
  for (var i = 0; i < gCnt; i++){
    if (i % 8 === 0 && !mobile){ gal1Frame.innerHTML += '<br>';}
    if (i % 6 === 0 && mobile){ gal1Frame.innerHTML += '<br>';}
    var blk = document.createElement('div');
    blk.className = 'ayteby8';
    blk.id = 'ayg'+indexNum[i];
    gal1Frame.appendChild(blk);
    newpath = createAyte(allGalHistory[i]);
    blk.innerHTML = svgStart+shaddow+newgroup+newpath+'</g></svg>';
  }
}

function createAyte(ayte){
  var newpath =''; var c = 0; var r = -1;
  for (var i = 0; i < 64; i++){
    if (i % 8 === 0){ c=1; r++;}
    var sx = (64*c)+2;
    var sy = (64*r)-2;
    var p = 68;
    if(ayte[i]!=="" && ayte[i] !=='rgb(221, 221, 221)' ){newpath += '<path fill-rule="evenodd" fill="'+ayte[i]+'" d="M'+sx+' '+sy+'v'+p+'h-'+p+'v-'+p+'h'+p+'z"/>';}
    c++;
  }
  return newpath;
}

function isInArray(value, array) {
  return array.indexOf(value) > -1;
}

function clearAllChildren(parentID){
  while (parentID.hasChildNodes()){
    parentID.removeChild(parentID.firstChild);
  }
}

// -------------------  pubnub's flex callback -------------------------------//
var historyGal = PUBNUB.init({
  publish_key: 'pub-c-a3bac365-84b3-4a6b-a8a1-67d0e2aaad3d',
  subscribe_key: 'sub-c-f0907bae-1ab6-11e6-9f24-02ee2ddab7fe'
});
historyGal.flex_history = pubnub_flex_history;
var indexNum = [];
var flex_history_callback = function(result) {
  if (!result.error) {
    var c = 0; gCnt = 0;
    for (var i = 0; i < result.count; i++){
      if (isInArray(i,skip)){ c++; }
      else { indexNum[gCnt] = i; gCnt++;
        allGalHistory[i-c] = result.messages[i].message.ayte
      }
    }
    //console.log("allGalHistory: ",allGalHistory);
    clearAllChildren(gal1Frame);
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
