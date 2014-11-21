var path = require("path");

var express = require("express");
var app = express();
var cookieParser = require("cookie-parser");
var session = require("express-session");

var passport = require('passport');
var LocalStrategy = require('passport-local').Strategy;
passport.use(new LocalStrategy(
    //TODO - you know actually authenticate against the DB here
    //for now just use this test user
    function(username, password, done) {
        var user = {
            userName: "not-implemented-yet"
        };
        done(null, user);
    }));

// need to store user stuff in session so we can pass the info to the websocket connection... not quite sure how yet.
// app.use(cookieParser());
// app.use(session({secret: '1234567890QWERTY'}));


// Http Server Configuration
var clientPath = path.normalize(__dirname + "../client");
app.use(express.static(clientPath));

//routes
app.get("/", function(req, res){
    res.sendFile(clientPath + "/index.html");
});

app.post("/login", function(req, res, next) {
    passport.authenticate('local', function(err, user, info) {
        if (err) {
            return next(err);
        }
        if (!user) {
            return res.write('login failed');
        }
        req.logIn(user, function(err) {
            if (err) {
                return next(err);
            }
            /*
            users[user.userName] = {
                status : "connected",
                viewing : "lobby",
                broguePID : -1
            };
            */
           
            return res.write('login success');
        });
    })(req, res, next);
});

app.post("/register", function(req, res){
    res.write("request for new user recieved");
});

server = app.listen(80, function(){
    
var port = server.address().port;
    console.log("http server listening on port %s", port);
});

// Web Socket Server
var wsPort = 8080;
var WebSocketServer = require("ws").Server;
var wss = new WebSocketServer({port: wsPort});
console.log("ws server listening on port %s", wsPort);

var brogue = require("./brogue");

var router = require("./router");
router.recieve.registerHandlers({
    "new" : brogue.spawn
});

wss.on("connection", function(ws) {
        
    var brogueSocket = new brogue.Socket(ws, router);
    
    ws.on("message", function(rawMsg){
       var msg = router.recieve.prepareData(rawMsg);
       router.recieve.route(msg);
       console.log("recieved %s", msg);
    });
    
});