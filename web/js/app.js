var mqtt;
var reconnectTimeout = 2000;
var host = "158.255.212.248";
var port = 8080;
var topic = "devlol/#";

window.onkeydown = function(e) {
    if (e.keyCode == 37 /* left */) {
        setMotor('left', '100');
        setMotor('right', '0');
    }
    if (e.keyCode == 39 /* right */) {
        setMotor('right', '100');
        setMotor('left', '0');
    }
    if (e.keyCode == 38 /* straight */) {
        setMotor('left', '100');
        setMotor('right', '100');
    }
    if (e.keyCode == 40 /* halt */) {
        setMotor('right', '0');
        setMotor('left', '0');
    }
}

function setMotor(motor, value) {
    mqtt.send("devlol/babyscooter/" + motor, value);
}

function MQTTconnect() {
    console.log("connecting");
    mqtt = new Paho.MQTT.Client(
                    host,
                    port,
                    "web_" + parseInt(Math.random() * 100,
                    10));
    var options = {
        timeout: 3,
        onSuccess: onConnect,
        onFailure: function (message) {
            setTimeout(MQTTconnect, reconnectTimeout);
        }
    };

    mqtt.onConnectionLost = onConnectionLost;
    mqtt.onMessageArrived = onMessageArrived;

    console.log("Host="+ host + ", port=" + port);
    mqtt.connect(options);
}

function onConnect() {
    console.log('Connected to ' + host + ':' + port);
    mqtt.subscribe(topic, {qos: 0});
    console.log(topic);
}

function onConnectionLost(response) {
    setTimeout(MQTTconnect, reconnectTimeout);
    console.log("connection lost: " + response.errorMessage + ". Reconnecting");
};

function onMessageArrived(message) {

    var topic = message.destinationName;
    var payload = message.payloadString;

    console.log(topic + ' = ' + payload);

    switch (topic) {
        case 'devlol/babyscooter/online':
            if (payload == "false") {
                document.body.style.backgroundColor = "red";
            }
            if (payload == "true") {
                document.body.style.backgroundColor = "black";
            }
            break;
        default:
            break;
    }
};


window.onload = function() {
    console.log("booting up");
    MQTTconnect();
};

