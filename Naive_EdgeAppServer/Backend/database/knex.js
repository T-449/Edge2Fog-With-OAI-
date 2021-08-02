const knex = require("knex");

const connectedKnex = knex({
    client: "sqlite3",
    connection: {
        filename: "users.sqlite3"
    }
});

module.exports = connectedKnex;