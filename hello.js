var addon = require('./build/Release/hello'),
    hello = addon.hello();

// Darek wuz here

console.log( hello() ); // 'world'