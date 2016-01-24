define([
    'jquery',
    'underscore',
    'backbone'
], function($, _, Backbone) {

    var UserModel = Backbone.Model.extend({
        defaults: {
            username : "",
            observing: false,
            recording: false,
            observingUsername: "",
            recordingId: 0,
            playing: false,
            loggedIn: false,
            showLobby: false
        }
    });

    return UserModel;

});