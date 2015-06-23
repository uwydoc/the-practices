// samegame.js
//
var maxRows = 15;
var maxColumns = 15;
var maxIndex = maxRows * maxColumns;
var blocks = new Array(maxIndex);
var component;

function indexOf(row, column)
{
    return column + row * maxColumns;
}

function isEmpty(block)
{
    return (block == null || block == undefined);
}

function startNewGame()
{
    // clean previous blocks
    for (var i = 0; i < maxIndex; ++i) {
        if (!isEmpty(blocks[i])) {
            blocks[i].destroy();
            blocks[i] = null;
        }
    }
    // update rows and columns according to current board size
    maxRows = Math.floor(gameCanvas.height / gameCanvas.blockSize);
    maxColumns = Math.floor(gameCanvas.width / gameCanvas.blockSize);
    if (maxRows * maxColumns != maxIndex) {
        maxIndex = maxRows * maxColumns;
        blocks = new Array(maxIndex);
    }
    // fill all blocks
    for (var column = 0; column < maxColumns; ++column) {
        for (var row = 0; row < maxRows; ++row) {
            var index = indexOf(row, column);
            var block = createBlock();
            if (block != null) {
                // properly position the newly created block
                var blockSize = gameCanvas.blockSize;
                block.x = column * blockSize;
                block.y = row * blockSize;
                block.width = blockSize;
                block.height = blockSize;
                block.type = Math.floor(Math.random() * 3);
                block.spawned = true;
                blocks[index] = block;
            } else {
                console.log("error creating the block");
            }
        }
    }
    // reset score
    gameCanvas.score = 0;
    score.text = "Score: " + gameCanvas.score;
}

function createBlock()
{
    // load component from qml file once
    if (component == null) {
        component = Qt.createComponent("Block.qml");
    }
    // Check load status
    if (component.status == Component.Ready) {
        var block = component.createObject(gameCanvas);
        if (block == null) {
            console.log("error creating object: " + component.errorString());
            return null;
        }
        return block;
    } else {
        console.log("error loading component: " + component.errorString());
        return null;
    }
}

function handleClick(x, y)
{
    var row = Math.floor(y / gameCanvas.blockSize);
    var column = Math.floor(x / gameCanvas.blockSize);
    if (isValidBlock(row, column)) {
        var score1 = removeSame(row, column);
        if (score1 > 0) {
            gameCanvas.score += score1;
            score.text = "Score: " + gameCanvas.score;
        }
    }
}

function isValid(row, column)
{
    return (row >= 0 && row < maxRows && column >= 0 && column < maxColumns);
}

function isValidBlock(row, column)
{
    return isValid(row, column) && blocks[indexOf(row, column)] != null;
}

function swapBlock(fromRow, fromCol, toRow, toCol)
{
    if (!isValid(fromRow, fromCol) || !isValid(toRow, toCol))
        return false;

    var fromIndex = indexOf(fromRow, fromCol);
    var toIndex = indexOf(toRow, toCol);
    var fromBlock = blocks[fromIndex];
    var toBlock = blocks[toIndex];
    if (fromBlock != null && toBlock != null) {
        var x = fromBlock.x;
        var y = fromBlock.y;
        fromBlock.x = toBlock.x;
        fromBlock.y = toBlock.y;
        toBlock.x = x;
        toBlock.y = y;
        blocks[fromIndex] = toBlock;
        blocks[toIndex] = fromBlock;
    } else if (fromBlock != null) {
        fromBlock.x = toCol * gameCanvas.blockSize;
        fromBlock.y = toRow * gameCanvas.blockSize;
        blocks[fromIndex] = null;
        blocks[toIndex] = fromBlock;
    } else if (toBlock != null) {
        toBlock.x = fromCol * gameCanvas.blockSize;
        toBlock.y = fromRow * gameCanvas.blockSize;
        blocks[fromIndex] = toBlock;
        blocks[toIndex] = null;
    }
    return true;
}

function arrayIndexOf(arr, val, f)
{
    f = f || function(a, b) { return (a == b); };
    for (var i = 0, l = arr.length; i < l; i++) {
        if (f(arr[i], val))
            return i;
    }
    return -1;
}

function sameBlocks(row, column)
{
    if (!isValidBlock(row, column))
        return [];

    var todo = [ [row, column] ];
    var done = [];
    var eq = function(a, b) { return (a[0] == b[0] && a[1] == b[1]); };
    while (todo.length > 0) {
        var item = todo.shift();
        if (arrayIndexOf(done, item, eq) != -1)
            continue;
        var r = item[0];
        var c = item[1];
        var t = blocks[indexOf(r, c)].type;
        var candidates = [ [r+1, c], [r-1, c], [r, c+1], [r, c-1] ];
        candidates.forEach(function(e) {
            if (isValidBlock(e[0], e[1])
                    && arrayIndexOf(done, e, eq) == -1
                    && arrayIndexOf(todo, e, eq) == -1
                    && blocks[indexOf(e[0], e[1])].type == t) {
                todo.push(e);
            }
        });
        done.push(item);
    }
    return done;
}

function removeSame(row, column)
{
    if (!isValidBlock(row, column))
        return 0;

    // remove same connected blocks
    var same = sameBlocks(row, column);
    var combo = same.length - 2;
    if (combo < 1)
        return 0;

    same.forEach(function(e) {
        var i = indexOf(e[0], e[1]);
        blocks[i].dying = true;
        blocks[i].destroy(200);
        blocks[i] = null;
    });
    var score = combo * combo;
    console.log("combo: " + combo + ", score: " + score);

    // flood fill removed blocks (down)
    for (var column = 0; column < maxColumns; ++column) {
        var m = maxRows - 1;
        var n = m;
        while (n >= 0) {
            while (m >= 0 && isValidBlock(m, column))
                m--;
            n = m - 1;
            while (n >= 0 && !isValidBlock(n, column))
                n--;
            var dist = 1;
            while (n-dist >= 0 && isValidBlock(n-dist, column))
                dist++;
            while (n >= 0 && dist--) {
                swapBlock(m, column, n, column);
                m--;
                n--;
            }
        }
    }
    // flood fill removed blocks (left)
    for (var row = 0; row < maxRows; ++row) {
        var m = 0;
        var n = m;
        while (n < maxColumns) {
            while (m < maxColumns && isValidBlock(row, m))
                m++;
            n = m + 1;
            while (n < maxColumns && !isValidBlock(row, n))
                n++;
            var dist = 1;
            while (n+dist < maxColumns && isValidBlock(row, n+dist))
                dist++;
            while (n < maxColumns && dist--) {
                swapBlock(row, m, row, n);
                m++;
                n++;
            }
        }
    }

    return score;
}
