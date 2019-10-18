
const busStat = 48; //in %
const carStat = 35; //in %
const cycleStat = 17; //in %
const lightsBus = 100; //number of LED lights
const lightsCar = 100; //number of LED lights
const lightsCycle = 100; //number of LED lights
var finalLED = []; //array to return acrylic panels to be lit up

function norm(countBus, countCar, countCycle) {
    var totalCount = (busStat+countBus) + (carStat+countCar) + (cycleStat+countCycle);
    var percentBus = ((busStat+countBus)/totalCount);
    var percentCar = ((carStat+countCar)/totalCount);
    var percentCycle = ((cycleStat+countCycle)/totalCount);

    finalLED.push(Math.round(percentBus*lightsBus));
    finalLED.push(Math.round(percentCar*lightsCar));
    finalLED.push(Math.round(percentCycle*lightsCycle));

    // console.log(finalLED);
    return finalLED;
}
