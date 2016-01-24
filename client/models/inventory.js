define([
    'jquery',
    'underscore',
    'backbone'
], function($, _, Backbone) {

    var InventoryModel = Backbone.Model.extend({
        defaults: {
            inventory: ""
        }
    });

    return InventoryModel;
});