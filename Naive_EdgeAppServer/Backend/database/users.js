const knex = require("./knex");

async function createUser(user) {
    const id = await knex("users").insert(user);
    return knex.select("id", "clicks").table("users").where("id", id);
}

async function updateClickCount(id, clicks) {
    await knex("users").where("id", id).update("clicks", clicks);
    return knex.select("id", "clicks").table("users").where("id", id);
}

function updateToken(id, token) {
    return knex("users").where("id", id).update("token", token);
}

function getToken(imsi) {
    return knex.select("token").table("users").where("imsi", imsi);
}

async function getClickCountOnIMSI(imsi) {
    return knex.select("clicks").table("users").where("imsi", imsi);
}

async function getClickCountOnID(id) {
    return knex.select("clicks").table("users").where("id", id);
}

async function getUserOnIMSI(imsi) {
    return knex.select("token", "clicks").table("users").where("imsi", imsi);
}

module.exports = {
    createUser,
    getClickCountOnIMSI,
    getClickCountOnID,
    updateClickCount,
    updateToken,
    getToken,
    getUserOnIMSI
}