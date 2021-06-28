var fhttp = new XMLHttpRequest();
var shttp = new XMLHttpRequest();

function showMenu() {
    var checkBox = document.getElementById("menuControl");
    var menuBody = document.getElementById("menuBody");
    var menuDiv = document.getElementById("menuDiv");

    if (checkBox.checked == true) {
        menuDiv.style.marginLeft = "5vh";
        menuBody.style.minWidth = "20vh";
    } 
    else {
        menuDiv.style.marginLeft = "-20vh";
        menuBody.style.minWidth = "0px";
    }
}

setInterval (function getFreq() {
    fhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var myObj = JSON.parse(this.responseText);
            setFreqHeight(myObj);
        }
    };

    fhttp.open("PUT", "freq", true);
    fhttp.send();
}, 100);

function mute() {
    var checkBox = document.getElementById("muteControl");
    var muteText = document.getElementById("muteText");

    if (checkBox.checked == true) {
        muteText.textContent = "Mute";
        shttp.open("POST", "mute=1", true);
    } 
    else {
        muteText.textContent = "Unmute";
        shttp.open("POST", "mute=0", true);
    }

    shttp.send();
}

function channel() {
    var checkBox = document.getElementById("channelControl");
    var channelText = document.getElementById("channelText");

    if (checkBox.checked == true) {
        channelText.textContent = "Left";
        shttp.open("POST", "chnl=1", true);
    } 
    else {
        channelText.textContent = "Right";
        shttp.open("POST", "chnl=0", true);
    }

    shttp.send();
}

function scale() {
    var slider = document.getElementById("scaleControl");

    shttp.open("POST", "scle=" + slider.value, true);
    shttp.send();
}

function ADC() {
    var slider = document.getElementById("ADCControl");

    shttp.open("POST", "ADCV=" + slider.value, true);
    shttp.send();
}

function DAC() {
    var slider = document.getElementById("DACControl");

    shttp.open("POST", "DACV=" + slider.value, true);
    shttp.send();
}

function setFreqHeight(obj) {
    document.getElementById("freq31").style.height = obj.freq31.toString() + "vh";
    document.getElementById("freq30").style.height = obj.freq30.toString() + "vh";
    document.getElementById("freq29").style.height = obj.freq29.toString() + "vh";
    document.getElementById("freq28").style.height = obj.freq28.toString() + "vh";
    document.getElementById("freq27").style.height = obj.freq27.toString() + "vh";
    document.getElementById("freq26").style.height = obj.freq26.toString() + "vh";
    document.getElementById("freq25").style.height = obj.freq25.toString() + "vh";
    document.getElementById("freq24").style.height = obj.freq24.toString() + "vh";
    document.getElementById("freq23").style.height = obj.freq23.toString() + "vh";
    document.getElementById("freq22").style.height = obj.freq22.toString() + "vh";
    document.getElementById("freq21").style.height = obj.freq21.toString() + "vh";
    document.getElementById("freq20").style.height = obj.freq20.toString() + "vh";
    document.getElementById("freq19").style.height = obj.freq19.toString() + "vh";
    document.getElementById("freq18").style.height = obj.freq18.toString() + "vh";
    document.getElementById("freq17").style.height = obj.freq17.toString() + "vh";
    document.getElementById("freq16").style.height = obj.freq16.toString() + "vh";
    document.getElementById("freq15").style.height = obj.freq15.toString() + "vh";
    document.getElementById("freq14").style.height = obj.freq14.toString() + "vh";
    document.getElementById("freq13").style.height = obj.freq13.toString() + "vh";
    document.getElementById("freq12").style.height = obj.freq12.toString() + "vh";
    document.getElementById("freq11").style.height = obj.freq11.toString() + "vh";
    document.getElementById("freq10").style.height = obj.freq10.toString() + "vh";
    document.getElementById("freq9").style.height = obj.freq9.toString() + "vh";
    document.getElementById("freq8").style.height = obj.freq8.toString() + "vh";
    document.getElementById("freq7").style.height = obj.freq7.toString() + "vh";
    document.getElementById("freq6").style.height = obj.freq6.toString() + "vh";
    document.getElementById("freq5").style.height = obj.freq5.toString() + "vh";
    document.getElementById("freq4").style.height = obj.freq4.toString() + "vh";
    document.getElementById("freq3").style.height = obj.freq3.toString() + "vh";
    document.getElementById("freq2").style.height = obj.freq2.toString() + "vh";
    document.getElementById("freq1").style.height = obj.freq1.toString() + "vh";
    document.getElementById("freq0").style.height = obj.freq0.toString() + "vh";
}