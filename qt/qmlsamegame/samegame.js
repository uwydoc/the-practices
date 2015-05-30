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
        gameCanvas.score += removeSame(row, column);
        score.text = "Score: " + gameCanvas.score;
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

function removeSame(row, column)
{
    if (!isValidBlock(row, column))
        return 0;

    var score = 0;
    // remove same connected blocks
    var todo = [ [row,column] ];
    var done = [];
    var combo = 1;
    while (todo.length > 0) {
        var item = todo.shift();
        var r = item[0];
        var c = item[1];
        var i = indexOf(r, c);
        if (done.indexOf(item) == -1) {
            if (blocks[i] != null) {
                var candidates = [ [r - 1, c], [r + 1, c], [r, c - 1], [r, c + 1] ];
                candidates.forEach(function(e) {
                    //console.log("candidate: " + e);
                    if (isValidBlock(e[0], e[1])
                        && blocks[indexOf(e[0], e[1])].type == blocks[i].type)
                        todo.push(e);
                });
                score += combo;
                combo++;
                // remove the item
                blocks[i].dying = true;
                blocks[i].destroy();
                blocks[i] = null;
            }
            done.push(item);
        }
    }
    // flood fill removed blocks
    for (var column = 0; column < maxColumns; ++column) {
        var f = 0;
        while (f < maxRows && !isValidBlock(f, column))
            f++;
        var m = f;
        var n = m;
        do {
            m = n;
            while (m < maxRows && isValidBlock(m, column))
                m++;
            n = m;
            while (n < maxRows && !isValidBlock(n, column))
                n++;
            var i = m - 1;
            var j = n - 1;
            //console.log("f: " + f + ", m: " + m + ", n: " + n);
            while (i >= f) {
                swapBlock(i, column, j, column);
                i--;
                j--;
            }
        } while (m > 0 && m < n && n <= maxRows);
    }

    return score;
}
