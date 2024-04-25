const express = require('express');
const app = express();

let status = '7';

app.get('/', (_, res) => {
    res.sendFile(__dirname + '/index.html');
});

app.get('/getStatus', (_, res) => {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.send(status);
});

app.get('/setStatus', (req, res) => {
    res.setHeader('Access-Control-Allow-Origin', '*');
    if(req.query.status >= 1 || req.query.status <= 7) {
        status = req.query.status;
        res.send('Status updated');
        return;
    }
    res.send('Invalid status');
});

app.listen(3000, () => {
    console.log('Server is running on port 3000');
});