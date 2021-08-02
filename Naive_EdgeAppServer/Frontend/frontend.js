const express = require("express");
const http = require("http");
const path = require('path');

const app = express();
app.use(express.json());
app.use(express.urlencoded({extended: true}));

const PORT = 45998;
const BACKEND_SERVER_PORT = process.env.PORT || 45999;
let currentToken;

function bin2String(array) {
    return String.fromCharCode.apply(String, array);
}

app.get('', (req, res) => {

    const userData = new TextEncoder().encode(
        JSON.stringify({
            'imsi': 320230100000025
        })
    );

    const options = {
        hostname: 'localhost',
        port: BACKEND_SERVER_PORT,
        path: '/register',
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'Content-Length': userData.length
        }
    };

    const postRequest = http.request(options, res => {
        console.log(`statusCode: ${res.statusCode}`);

        res.on('data', d => {
            let output = JSON.parse(bin2String(d));
            currentToken = output.token;
            console.log('Clicks:' + output.clicks);
            console.log('Token: ' + output.token);
        });
    });

    postRequest.on('error', error => {
        console.error(error)
    });
      
    postRequest.write(userData);

    postRequest.end();

    res.sendFile(path.join(__dirname, "/counter.html"));
})

app.get('/click', (req, res) => {

    const token = "Bearer " + currentToken;

    const options = {
        hostname: 'localhost',
        port: BACKEND_SERVER_PORT,
        path: '/click',
        method: 'GET',
        headers: {
            'Authorization': token
        }
    };

    const getRequest = http.request(options, res => {

        console.log(`statusCode: ${res.statusCode}`);
    
        res.on('data', d => {
            let output = JSON.parse(bin2String(d));
            currentToken = output.token;
            console.log("Clicks: " + output.clicks);
            console.log('Token: ' + output.token);
        });
    });
    
    getRequest.on('error', error => {
        console.error(error)
    });
          
    getRequest.end();

    res.sendFile(path.join(__dirname, "/counter.html"));
});


app.listen(PORT, () => {

    console.log("Listening on port " + PORT);
})