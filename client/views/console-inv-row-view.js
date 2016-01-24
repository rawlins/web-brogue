define([
    "jquery",
    "underscore",
    "backbone",
    "moment"
], function ($, _, Backbone) {

    var ConsoleInventoryRowView = Backbone.View.extend({
        tagName: "li",
        className: "console-inv-row",

        template : _.template($('#console-inv-row-template').html()),

        render: function() {

            this.$el.html(this.template(this.model.toJSON()));
            return this;
        },
    });

    return ConsoleInventoryRowView;

});