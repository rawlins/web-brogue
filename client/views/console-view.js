// View for the entire console.  It is responsible for setting up all of the console-cell views

define([
    "jquery",
    "underscore",
    "backbone",
    "dispatcher",
    "pixi",
    'dataIO/send-keypress',
    "views/console-cell-view",
    "models/console-cell",
    "views/view-activation-helpers"
], function($, _, Backbone, dispatcher, PIXI, sendKeypressEvent, ConsoleCellView, CellModel, activate) {

    var _CONSOLE_ROWS = 34;
    var _CONSOLE_COLUMNS = 100;
    var _MESSAGE_UPDATE_SIZE = 10;

    var _consoleCells = [];
    var _consoleWidth;
    var _consoleHeight;
    var _consoleCellTopOffsetPercent;
    var _consoleCellLeftOffsetPercent;
    var _consoleCellWidthPercent;
    var _consoleCellHeightPercent;
    var _consoleCellCharSizePx;
    var _consoleCellCharPaddingPx;
    var _consoleCellAspectRatio = 0.53;  //TODO: we may eventually want this to be adjustable

    var Console = Backbone.View.extend({
        el: "#console",
        events: {
            'focus': 'giveKeyboardFocus'
        },
        rectangle: function (x, y, width, height, backgroundColor)
        {
            var box = new PIXI.Graphics();
            box.beginFill(backgroundColor);
            box.drawRect(0, 0, width, height);
            box.endFill();
            box.position.x = x;
            box.position.y = y;
            //console.log("x: " + box.position.x + " y: " + box.position.y);
            return box;
        },
        initialize: function() {

            this.calculateConsoleSize();
            this.calculateConsoleCellSize();

            this.initializeConsoleCells();

            var canvasWidth = 1200;
            var canvasHeight = 600;

            var renderer = PIXI.autoDetectRenderer(canvasWidth, canvasHeight,{backgroundColor : 0x1099bb});
            this.$el.append(renderer.view);

            // create the root of the scene graph
            var stage = new PIXI.Container();

            //var basicText = new PIXI.Text('Basic text in pixi');
            //basicText.x = 30;
            //basicText.y = 90;

            //stage.addChild(basicText);

            //var assetsToLoader = [];
            /*
            var loader = PIXI.loader; // pixi exposes a premade instance for you to use.
//or
            var loader = new PIXI.loaders.Loader(); // you can also create your own if you want

            loader.add('couriernew.xml');

            loader.once('complete',onAssetsLoaded);

            loader.load();
            */
            function onAssetsLoaded()
            {
                // start animating
                animate();
            }

            var self = this;

            function animate() {

                //console.log("running frame");
                var requireRedraw = false;

                for (var i = 0; i < _CONSOLE_COLUMNS; i++) {
                    for (var j = 0; j < _CONSOLE_ROWS; j++) {

                        var cellModel = _consoleCells[i][j].model;

                        if (cellModel.get("dirty")) {

                            //console.log("rendering dirty");

                            var rgbForegroundString = "rgb(" +
                                cellModel.get("foregroundRed") + "," +
                                cellModel.get("foregroundGreen") + "," +
                                cellModel.get("foregroundBlue") + ")";
                            var rgbBackgroundString = "rgb(" +
                                cellModel.get("backgroundRed") + "," +
                                cellModel.get("backgroundGreen") + "," +
                                cellModel.get("backgroundBlue") + ")";

                            var backgroundColor = (256 * 256) * cellModel.get("backgroundRed") +
                                256 * cellModel.get("backgroundGreen") +
                                cellModel.get("backgroundBlue");


                            var cellChar = String.fromCharCode(cellModel.get("char"));

                            var width = cellModel.get("widthPercent");
                            var height = cellModel.get("heightPercent");
                            var left = cellModel.get("leftPositionPercent");
                            var top = cellModel.get("topPositionPercent");
                            var fontSize = (cellModel.get("charSizePx") ).toString() + "px";
                            var paddingTop = cellModel.get("charPaddingPx") + "px";

                            var fontStr = fontSize + " " + '"Lucida Console", Monaco, monospace';
                            //console.log(fontStr);

                            var style = {
                                font: fontStr,
                                fill: rgbForegroundString
                            };

                            //console.log(cellChar);
                            //console.log(JSON.stringify(style));

                            if (cellModel.get("charSizePx") > 0) {

                                var cellX = canvasWidth * left / 100;
                                var cellY = canvasHeight * top / 100;

                                var cellWidth = canvasWidth * width / 100;
                                var cellHeight = canvasHeight * height / 100;

                                //Cleanup
                                if (_consoleCells[i][j].model.get("pixiRect")) {
                                    stage.removeChild(_consoleCells[i][j].model.get("pixiRect"));
                                }

                                if (_consoleCells[i][j].model.get("pixiText")) {
                                    stage.removeChild(_consoleCells[i][j].model.get("pixiText"));
                                }

                                //Background
                                var cellRect = self.rectangle(cellX, cellY, cellWidth, cellHeight, backgroundColor);
                                stage.addChild(cellRect);

                                //Foreground text
                                var cellText = new PIXI.Text(cellChar, style); //very slow
                                cellText.x = cellX;
                                cellText.y = cellY;

                                //console.log("x: " + cellText.x + " y: " + cellText.y);
                                stage.addChild(cellText);

                                requireRedraw = true;

                                _consoleCells[i][j].model.set({
                                    "pixiText": cellText,
                                    "pixiRect": cellRect,
                                    "dirty": false });
                            }
                        }
                    }
                }

                requestAnimationFrame(animate);

                // render the root container
                if(requireRedraw) {
                    renderer.render(stage);
                }
            }

            animate();

            this.$el.addClass("full-height");

        },
        initializeConsoleCells: function() {

            //var consoleCellsFragment = document.createDocumentFragment();
            
            for (var i = 0; i < _CONSOLE_COLUMNS; i++) {
                var column = [];
                for (var j = 0; j < _CONSOLE_ROWS; j++) {
                    var cellModel = new CellModel({
                        x: i,
                        y: j,
                        widthPercent: _consoleCellWidthPercent,
                        heightPercent: _consoleCellHeightPercent,
                        charSizePx: _consoleCellCharSizePx,
                        charPaddingPx: _consoleCellCharPaddingPx,
                        topOffsetPercent: _consoleCellTopOffsetPercent,
                        leftOffsetPercent: _consoleCellLeftOffsetPercent
                    });

                    var cellView = new ConsoleCellView({
                        model: cellModel,
                        id: "console-cell-" + i + "-" + j
                    });

                    //consoleCellsFragment.appendChild(cellView.render().el);
                    column.push(cellView);
                }
                _consoleCells.push(column);
            }
            
            //this.$el.append(consoleCellsFragment);

        },
        calculateConsoleSize: function() {
            _consoleWidth = this.$el.width();
            _consoleHeight = this.$el.height();
        },
        calculateConsoleCellSize: function() {

            _consoleCellWidthPercent = 100 / _CONSOLE_COLUMNS;

            // Cell Aspect Ratio
            var cellPixelWidth = _consoleWidth * (_consoleCellWidthPercent / 100);
            var cellPixelHeight = cellPixelWidth / _consoleCellAspectRatio;

            //If this height will make the console go off screen, recalculate size and horizontally center instead
            if (cellPixelHeight * _CONSOLE_ROWS > _consoleHeight) {              
                cellPixelHeight = _consoleHeight / _CONSOLE_ROWS;
                cellPixelWidth = cellPixelHeight * _consoleCellAspectRatio;

                _consoleCellHeightPercent = 100 / _CONSOLE_ROWS;
                _consoleCellWidthPercent = 100 * cellPixelWidth / _consoleWidth;
                _consoleCellTopOffsetPercent = 0;

                var leftOffSetPx = (_consoleWidth - cellPixelWidth * _CONSOLE_COLUMNS) / 2;
                _consoleCellLeftOffsetPercent = leftOffSetPx / _consoleWidth * 100;
            }
            else {
                // Vertically center the console
                _consoleCellHeightPercent = 100 * cellPixelHeight / _consoleHeight;
                _consoleCellLeftOffsetPercent = 0;
                var topOffSetPx = (_consoleHeight - cellPixelHeight * _CONSOLE_ROWS) / 2;
                _consoleCellTopOffsetPercent = topOffSetPx / _consoleHeight * 100;
            }

            // Cell Character Positioning
            _consoleCellCharSizePx = cellPixelHeight * 3 / 5;
            _consoleCellCharPaddingPx = cellPixelHeight / 10;
        },
        render: function() {
            for (var i = 0; i < _CONSOLE_COLUMNS; i++) {
                for (var j = 0; j < _CONSOLE_ROWS; j++) {
                    _consoleCells[i][j].render();
                }
            }
        },

        resize: function() {
            this.calculateConsoleSize();
            this.calculateConsoleCellSize();
            for (var i = 0; i < _CONSOLE_COLUMNS; i++) {
                for (var j = 0; j < _CONSOLE_ROWS; j++) {
                    _consoleCells[i][j].model.set({
                        widthPercent: _consoleCellWidthPercent,
                        heightPercent: _consoleCellHeightPercent,
                        charSizePx: _consoleCellCharSizePx,
                        charPaddingPx: _consoleCellCharPaddingPx,
                        topOffsetPercent: _consoleCellTopOffsetPercent,
                        leftOffsetPercent: _consoleCellLeftOffsetPercent
                    });
                    _consoleCells[i][j].model.calculatePositionAttributes();
                    _consoleCells[i][j].applySize();
                    _consoleCells[i][j].model.set({
                        "dirty": true });
                }
            }
        },
        
        queueUpdateCellModelData : function(data){
            // todo -- comment
            var self = this;
            setTimeout(function(){
                self.updateCellModelData(data);
            }, 0);
        },
        updateCellModelData: function (data) {
            var dataArray = new Uint8Array(data);
            var dataLength = dataArray.length;
            var dIndex = 0;

            while (dIndex < dataLength) {
                var dataXCoord = dataArray[dIndex++];
                var dataYCoord = dataArray[dIndex++];

                // Status updates have coords (255,255). For now ignore these, eventually we may find a UI use for them
                if (dataXCoord === 255 && dataYCoord === 255){
                    dIndex += _MESSAGE_UPDATE_SIZE - 2;
                    continue;
                }

                var combinedUTF16Char = dataArray[dIndex++] << 8 | dataArray[dIndex++];

                _consoleCells[dataXCoord][dataYCoord].model.set({
                    char: combinedUTF16Char,
                    foregroundRed: dataArray[dIndex++],
                    foregroundGreen: dataArray[dIndex++],
                    foregroundBlue: dataArray[dIndex++],
                    backgroundRed: dataArray[dIndex++],
                    backgroundGreen: dataArray[dIndex++],
                    backgroundBlue: dataArray[dIndex++],
                    dirty: true
                });

                //_consoleCells[dataXCoord][dataYCoord].render();
            }
        },
        
        clearConsole : function(){
            for (var i = 0; i < _CONSOLE_COLUMNS; i++) {
                for (var j = 0; j < _CONSOLE_ROWS; j++) {
                    _consoleCells[i][j].model.clear();
                    _consoleCells[i][j].render();
                }
            }
        },
        
        giveKeyboardFocus : function(){
            $('#console-keyboard').focus();
        },
        
        exitToLobby : function(message){
            activate.lobby();
            activate.currentGames();
            dispatcher.trigger("leaveGame");
            this.clearConsole();
        }
    });

    return Console;

});
