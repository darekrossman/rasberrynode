var addon = require('./build/Release/hello'),
    hello = addon.hello();

console.log( hello() + ' World' ); // 'world'