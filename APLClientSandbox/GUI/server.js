#!/usr/bin/env node
/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

const path = require('path');
const express = require('express');
const fs = require('fs')
const app = express();

const INTERFACE = '127.0.0.1';
const PORT = 8000;

const LIB_PATH = path.join(__dirname, 'node_modules', 'apl-client');
const STATIC_PATH = path.join(__dirname);

const getExtraPath = () => {
    const args = process.argv.slice(2);
    return  (args.length > 0) ? args[0] : undefined;
};

const logRequest = (req, res, next) => {
    console.log(`${req.method} ${req.originalUrl}`);
    next();
};

// routes
app.use(logRequest);

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();
});

app.use(express.static(LIB_PATH));
app.use(express.static(STATIC_PATH));

// startup
const extraPath = getExtraPath();
if (extraPath) {
    app.use(express.static(extraPath));
    console.log('Adding ' + extraPath + ' to /');
}

app.get('/templates', (req, res) => {
    var examples = [];
    var root = "public"
    var documents = new Set();
    fs.readdirSync(path.join(root, "templates")).forEach(file => documents.add(file));
    documents.forEach(filename => {
        if (path.extname(filename) === '.json') {
            examples.push({
                name: filename
            })
        }
    });
    res.set('Cache-Control', 'no-cache, no-store, must-revalidate').send(examples);
});

app.get("/templates/*.json", async (req, res) => {
    var root = "public"
    fs.readFile(path.join(root, req.originalUrl), 'utf8', (err, template) => {
        if (err) {
            return res.status(404).send(`Could not load template at ${req.originalUrl}`);
        }
        res.set('Cache-Control', 'no-cache, no-store, must-revalidate').send(template);
    });
});

app.get("/svg/*.svg", async (req, res) => {
    var root = "public"
    fs.readFile(path.join(root, req.originalUrl), 'utf8', (err, template) => {
        if (err) {
            return res.status(404).send(`Could not load template at ${req.originalUrl}`);
        }
        res.set('Cache-Control', 'no-cache, no-store, must-revalidate').send(template);
    });
});

console.log('Listening on ' + INTERFACE  + ':' + PORT);
app.listen(PORT, INTERFACE);

