define([
    "jquery",
    "underscore",
    "backbone",
    "dispatcher"
], function ($, _, Backbone, dispatcher) {

    var ConsoleInventoryView = Backbone.View.extend({
        el: "#console-inv",
        template: _.template($('#console-inv-template').html()),

        initialize: function () {
            this.render();
        },
        render: function () {

            this.$el.html(this.template(this.model.toJSON()));
        },

        updateInventory: function (data) {

            console.log("Inventory update");
            this.model.set("inventory", data);
            this.render(); //should be unnecessary - need to set up watch
        }
    });

    return ConsoleInventoryView;

});
