define([
    "jquery",
    "underscore",
    "backbone",
    "../models/inventory-entry",
    "../views/console-inv-row-view"
], function ($, _, Backbone, InventoryEntryModel, ConsoleInventoryRowView) {

    var ConsoleInventoryView = Backbone.View.extend({
        el: "#console-inv",
        template: _.template($('#console-inv-template').html()),

        initialize: function () {
            this.render();
        },
        render: function () {

            var i = 0;
            var rawStr = this.model.get("inventory");
            var cleanInvStr = "";
            var inventoryEntries = [];

            while (i < rawStr.length) {
                var c = rawStr.charAt(i);
                var code = rawStr.charCodeAt(i);
                if(code == 25) {
                    i += 4;
                }
                else if(code == 10) {
                    inventoryEntries.push(new InventoryEntryModel({"item": cleanInvStr}));
                    cleanInvStr = "";
                    i++;
                }
                else {
                    cleanInvStr += c;
                    i++;
                }
            }

            this.$el.html(this.template());

            this.$listElement = this.$el.find('#console-inv-list');

            _.each(inventoryEntries, function (model) {
                var item = new ConsoleInventoryRowView({model: model});
                this.$listElement.append(item.render().$el);
            }, this);
        },

        updateInventory: function (data) {

            console.log("Inventory update");
            this.model.set("inventory", data);
            this.render(); //should be unnecessary - need to set up watch
        }
    });

    return ConsoleInventoryView;

});
