const express = require("express");
const database = require("./database/users");
const jwt = require('jsonwebtoken');

const app = express();
app.use(express.json());

const PORT = process.env.PORT || 45999;
const SECRET_KEY = "My Key Is A Very Big Secret";

function makeToken(userData, res) {

    jwt.sign({ userData: userData }, SECRET_KEY, async (err, token) => {

        if(err) {
            res.status(404).send("Cannot create JWT");
            return;
        }

        await database.updateToken(userData[0]["id"], token);
        res.status(200).json({ token: token, clicks: userData[0]["clicks"]  });
    });
}

function verifyToken(req, res, next){
    const bearerHeader = req.headers['authorization'];
    if(typeof bearerHeader !== 'undefined') {
        const bearerToken = bearerHeader.split(' ')[1];
        req.token = bearerToken;
        next();
    } else {
        res.sendStatus(403);
    }
}

app.post('/register', async(req, res) => {
    
    const userData = await database.getUserOnIMSI(req.body.imsi);
    if(Object.keys(userData).length !== 0) {
        res.status(200).json({ token: userData[0]["token"], clicks: userData[0]["clicks"]  });
        return;
    }

    const newUser = {
        'imsi': req.body.imsi,
        'clicks': 0
    };

    const userdata = await database.createUser(newUser);
    makeToken(userdata, res);
}) ;

app.get('/click', verifyToken, (req, res) => {

    jwt.verify(req.token, SECRET_KEY, async (err, authData) => {
    
        if(err) {
            res.status(403).send("Invalid JWT");
            return;
        } else {
            let clickCount = await database.getClickCountOnID(authData["userData"][0]["id"]);
            if(clickCount[0]["clicks"] ===  authData["userData"][0]["clicks"]) {
                clickCount[0]["clicks"] = clickCount[0]["clicks"] + 1;
                const userData = await database.updateClickCount(authData["userData"][0]["id"], clickCount[0]["clicks"]);
                makeToken(userData, res);
            } else {
                res.status(403).send("Corrupted JWT");
                return;
            }
                
        }    
    });
})

app.get('/clickCount', async (req, res) => {
    const clickCount = await database.getClickCountOnIMSI(req.query.imsi)
    console.log(clickCount)
    res.status(200).json({ clicks: clickCount[0]["clicks"] });
    return;
});

app.listen(PORT, () => {

    console.log(`Listening on port ${PORT} ......`);

});
