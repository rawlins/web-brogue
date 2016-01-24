define([
    'jquery',
    'underscore',
    'backbone'
], function($, _, Backbone) {

    var InventoryEntryModel = Backbone.Model.extend({
        defaults: {
            item: ""
        }
    });

    return InventoryEntryModel;
});