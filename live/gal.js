var svgElement = 'http://www.w3.org/2000/svg';
var allGalHistory = {};
var stateGal = false;
var gCnt = 0;
var skip = [0,1,21,27,28,29,32];

function swapToGal(){
  if (!stateGal){ stateGal = true;
    if (live) {liveStatus();}
    console.log('swapped to gallery Page');
    galName.style.display = 'block';
    gal1Frame.style.display = 'block';
    ayteFrame.style.display = 'none';
    paletteFrame.style.display = 'none';
    //gal1.style.display = 'block';
    galBtn.innerHTML = '<-Back'
    historyGal.flex_history(getAll, flex_history_callback);
  } else { stateGal = false;
    galName.style.display = 'none';
    gal1Frame.style.display = 'none';
    ayteFrame.style.display = 'block';
    paletteFrame.style.display = 'block';
    //gal1.style.display = 'none';
    galBtn.innerHTML = '{Gallery}'

  }
}

var svgStart = '<svg xmlns="http://www.w3.org/2000/svg" width="9.5em" height="9.5em" viewBox="0 0 528 528" >';

var shaddow = '<defs><filter id="f1" height="130%" width="130%"><feGaussianBlur in="SourceAlpha" stdDeviation="5"/> <feOffset dx="5" dy="5" result="offsetblur"/><feComponentTransfer><feFuncA type="linear" slope="0.5"/></feComponentTransfer><feMerge><feMergeNode/><feMergeNode in="SourceGraphic"/></feMerge></filter></defs>';
var newgroup = '<g filter="url(#f1)" >';
//nooh
function buildGal(){
  for (var i = 0; i < gCnt; i++){
    if (i % 8 === 0 && !mobile){ gal1Frame.innerHTML += '<br>';}
    if (i % 8 === 0 && mobile){ gal1Frame.innerHTML += '<br>';}
    var blk = document.createElement('div');
    blk.className = 'ayteby8';
    blk.id = 'ayg'+i;//BUILD NEW ARRAY AT HISTORY FLEX TO GET ACTUAL POSITION IN API, easy to find what ayte to remove with hover.
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
    if(ayte[i]!==""){newpath += '<path fill-rule="evenodd" fill="'+ayte[i]+'" d="M'+sx+' '+sy+'v'+p+'h-'+p+'v-'+p+'h'+p+'z"/>';}
    c++;
  }
  //var newpath = '<path fill-rule="evenodd" fill="green" d="M64 0v64h-64v-64h64z"/><path fill-rule="evenodd" fill="green" d="M64 448v64h-64v-64h64z"/><path fill-rule="evenodd" fill="blue" d="M512 448v64h-64v-64h64z"/><path fill-rule="evenodd" fill="green" d="M512 0v64h-64v-64h64z"/><path fill-rule="evenodd" fill="blue" d="M256 0v64h-64v-64h64z"/><path fill-rule="evenodd" fill="blue" d="M128 64v64h-64v-64h64z"/><path fill-rule="evenodd" fill="blue" d="M128 128v64h-64v-64h64z"/><path fill-rule="evenodd" fill="blue" d="M512 256v64h-64v-64h64z"/><path fill-rule="evenodd" fill="red" d="M512 192v64h-64v-64h64z"/><path fill-rule="evenodd" fill="green" d="M128 448v64h-64v-64h64z"/><path fill-rule="evenodd" fill="cornflowerblue" d="M256 256v64h-64v-64h64z"/><path fill-rule="evenodd" fill="cornflowerblue" d="M256 320v64h-64v-64h64z"/><path fill-rule="evenodd" fill="cornflowerblue" d="M320 256v64h-64v-64h64z"/><path fill-rule="evenodd" fill="cornflowerblue" d="M320 320v64h-64v-64h64z"/>';
  return newpath;
}

//var w = 8; //var w=15; var gap=1.5; var rnd=3; LOOKS GOOD FOR MEDIUM THUMB
//var gap = -0.75;
//var rnd = 0; //remove!!!
//var filter = 'url(#f1)';
// function buildGal(){
//   var col = -1; var row = -8;
//   for (var i = 0; i < allGalHistory[0].length; i++){
//     if (i % 8 === 0){ row+=8; col++}
//     //need to build a container so that we can have click effects....
//     var container = 'gal1';
//     buildThumbnail(container, i, (i-row)*((w+gap)*10), ((w+gap)*10)*col);
//   }
// }
//
// function buildThumbnail(container, ayte, cA, rA){
//   var col = 0; var row = -8;
//   for (var i = 0; i < 64; i++){
//     if (i % 8 === 0){ row+=8; col++}
//     var c = allGalHistory[ayte][i];
//     if (c !== "" && c !== 'rgb(221, 221, 221)'){
//         createRect('gal1',cA+((w+gap)*(i-row)),rA+((w+gap)*col),w,w,rnd,rnd
//         ,c,0,'none',filter,'aytep'+i);
//     }
//   }
// }
//
// function createRect(container,x,y,width,height,rx,ry,fill,bordWidth,bordColor,filter,recID){
//   var newEl = createEl(container,'rect',[['id',recID],['stroke-width',bordWidth+'px']
//   ,['stroke',bordColor],['x',x+'px'],['y', y+'px'],['width',width+'px'],['height',height+'px']
//   ,['filter', filter]
//   ,['rx',rx],['ry',ry],['fill',fill]]); return newEl;}
//
// function createEl(container,type,att){
//   var newObj = document.createElementNS(svgElement, type);
//   for (var i=0; i<att.length; i++){ newObj.setAttributeNS(null, att[i][0],att[i][1]); }
//   document.getElementById(container).appendChild(newObj); return newObj; }

function isInArray(value, array) {
  return array.indexOf(value) > -1;
}

// -------------------  pubnub's flex callback -------------------------------//
var historyGal = PUBNUB.init({
  publish_key: 'pub-c-a3bac365-84b3-4a6b-a8a1-67d0e2aaad3d',
  subscribe_key: 'sub-c-f0907bae-1ab6-11e6-9f24-02ee2ddab7fe'
});
historyGal.flex_history = pubnub_flex_history;

var flex_history_callback = function(result) {
  if (!result.error) {
    console.log(result.operation + " completed", result);
    var c = 0;
    for (var i = 0; i < result.count; i++){
      if (isInArray(i,skip)){ c++; }
      //need to give id as i so that it's easy to inspect in console to add to skip arrey
      else { gCnt++;
        allGalHistory[i-c] = result.messages[i].message.ayte}
    }
    console.log("allGalHistory: "+allGalHistory);
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
