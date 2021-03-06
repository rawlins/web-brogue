// Model for user information and settings. 
// Right now it is only used in the header logout button, but the plan is to extend to include other user-specific options

// TODO- this is similar to the auth model merge?

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