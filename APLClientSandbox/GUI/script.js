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

class WebsocketConnection {
    constructor(messageCallback) {
        this.connected = false;
        this.callback = messageCallback;
        this.connect();
    }

    connect() {
        const self = this;

        const socketUrl = (
            (['localhost', '127.0.0.1', '0.0.0.0'].includes(window.location.hostname))
            ? `ws://${window.location.hostname}:8080` : `ws://${window.location.host}`
        );

        this.socket = new WebSocket(socketUrl);

        this.socket.onopen = function(e) {
            console.debug("[Websocket] Connection established");
            self.connected = true;
        };

        this.socket.onmessage = function(event) {
            self.callback(JSON.parse(event.data));
            console.debug(`[Websocket] Data received from server: ${event.data}`);
        };

        this.socket.onclose = function(event) {
            self.connected = false;
            if (event.wasClean) {
                console.info(`[Websocket] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
            } else {
                console.warn('[Websocket] Connection died');
            }
            window.setTimeout(self.connect.bind(self), 1000);
        };

        this.socket.onerror = function(error) {
            console.error(`[Websocket] ${error.message}`);
            // reconnect will be handled by onclose
        };
    }

    disconnect() {
        this.socket.close();
    }

    send(message) {
        if (this.connected) {
            if (typeof message != "string") {
                this.socket.send(JSON.stringify(message))
            } else {
                this.socket.send(message);
            }
        } else {
            console.error("[Websocket] No active connection");
        }
    }
}

class Client extends APLClient.APLClient {
    constructor(socket) {
        super();
        this.socket = socket;
    }

    sendMessage(message) {
        this.socket.send({
            type: 'apl',
            payload: message
        });
    }
}

const formatMetricLog = (date, metric) => {
    const { name, value } = metric;
    const identifier = 'APL-WebViewhostMetrics';

    const month = ('0' + (date.getMonth() + 1)).slice(-2);
    const day = ('0' + date.getDate()).slice(-2);

    const hours = date.getHours().toString().padStart(2, '0');
    const minutes = date.getMinutes().toString().padStart(2, '0');
    const seconds = date.getSeconds().toString().padStart(2, '0');
    const milliseconds = date.getMilliseconds().toString().padStart(3, '0');

    return `${month}-${day} ${hours}:${minutes}:${seconds}.${milliseconds} | ${identifier} : name="${name}" value=${value}`;
};

const handleMessage = (data) => {
    switch(data.type) {
        case 'reset':
            resetViewhost();
            break;
        case 'viewhost':
            const payload = JSON.parse(data.payload);
            if (payload.type === 'metric') {
                const metric = payload.payload;
                return console.log(formatMetricLog(new Date(), metric));
            }
            client.onMessage(JSON.parse(data.payload));
            break;
        case 'resourcerequest':
            handleResourceRequest(data.payload);
            break;
        default:
            console.error("Unknown message type");
            break;
    }
};

const socket = new WebsocketConnection(handleMessage);
const client = new Client(socket);
let rendererElement;
let renderer;

const load = () => {
    for (const it of ['aplTemplate', 'aplDataModel', 'viewports', 'aplCommand', 'height', 'width', 'dpi', 'scaling', 'mode', 'docTheme']) {
        if (localStorage.getItem(it)) {
            document.getElementById(it).value = localStorage.getItem(it);
        }
    }
    document.getElementsByClassName('tab')[0].click();
};

const handleResourceRequest = (source) => {
    console.log('retrieving resource', source);
    fetch(source).then((response) => {
        if (!response.ok) {
            socket.send({
                type: 'resourceresponse',
                url: source,
                payload: ''
            });
        } else {
            response.text().then((data) => {
                socket.send({
                    type: 'resourceresponse',
                    url: source,
                    payload: data
                });
            });
        }
    })
};

const resetViewhost = () => {
    console.log('resetViewhost');
    const height = parseInt(document.getElementById('height').value);
    const width = parseInt(document.getElementById('width').value);
    const dpi = parseInt(document.getElementById('dpi').value);
    const mode = document.getElementById('mode').value;
    const theme = document.getElementById('docTheme').value;
    const environment = {
        agentName: 'APLClientSandbox',
        agentVersion: '1.0',
        disallowVideo: false,
        allowOpenUrl: false,
        animationQuality: 'normal'
    };

    const viewport = {
        /** Width in pixels */
        width,
        /** Height in pixels */
        height,
        /** `true` if the screen is round */
        isRound : false,
        /** Dots per inch */
        dpi
    };

    const extensions = document.getElementsByClassName('extension');
    const supportedExtensions =
        Array.from(extensions)
            .filter((ext) => ext.checked)
            .map((ext) => ext.getAttribute('data-extension-uri'))
    supportedExtensions.push('aplext:e2eencryption:10');

    const options = {
        view: rendererElement,
        theme,
        viewport,
        mode,
        environment,
        client,
        supportedExtensions,
        onResizingIgnored: resizingIgnored,
        developerToolOptions: {
            includeComponentId: true
        },
        scrollCommandDuration: parseInt(document.getElementById('scrollDuration').value)
    };

    if (renderer) {
        renderer.destroy();
    }
    waitForFirstMeaningfulPaint(30, 2000);
    // Reset rotation
    rendererElement.style.transition = 'unset';
    rendererElement.style.transform = 'rotate(0deg)';
    rendererElement.style.transformOrigin = '100% 0%';

    renderer = APLClient.APLWSRenderer.create(options);
    renderer.init().then(() => {
        console.log(`APL Renderer init resolved with viewport: \
        ${renderer.context.getViewportWidth()} x ${renderer.context.getViewportHeight()}`);
        rendererElement.style.transition = 'transform 250ms ease-in-out';
    });
};

function toNanoseconds(milliseconds) {
    // DOMHighResTimeStamps are accurate to within 5 microseconds
    const microseconds = Math.round(milliseconds * 1000);
    return microseconds * 1000;
}

const RENDER_DOCUMENT_START = 'renderClick';
const RENDER_DOCUMENT_END = 'firstMeaningfulPaint';
const RENDER_DOCMENT_METRIC = 'APL-Web.renderDocument';

function waitForFirstMeaningfulPaint(delayMs, tries) {
    const keepWaiting = () => {
        setTimeout(() => {
                waitForFirstMeaningfulPaint(delayMs, tries - 1)
            },
            delayMs);
    }

    if (rendererElement && rendererElement.querySelector('p')) {
        performance.mark(RENDER_DOCUMENT_END);
        performance.measure(RENDER_DOCMENT_METRIC, RENDER_DOCUMENT_START, RENDER_DOCUMENT_END);
        const renderDocumentEntries = performance.getEntriesByName(RENDER_DOCMENT_METRIC)
        for (let i = 0; i < renderDocumentEntries.length; i++) {
            const metric = {
                name: RENDER_DOCMENT_METRIC,
                value: toNanoseconds(renderDocumentEntries[i].duration)
            }
            console.log(formatMetricLog(new Date(), metric));
        }
        performance.clearMeasures(RENDER_DOCMENT_METRIC);
    } else if (tries > 0) {
        keepWaiting();
    } else {
        console.error('Could not detect meaningful paint.');
    }
}

async function getVisualContext() {
    console.log(await renderer.context.getVisualContext());
}

async function getDataSourceContext() {
    console.log(await renderer.context.getDataSourceContext());
}

const renderDocument = () => {
    const doc = document.getElementById('aplTemplate').value;
    const data = document.getElementById('aplDataModel').value;
    const viewports = document.getElementById('viewports').value;
    const height = parseInt(document.getElementById('height').value);
    const width = parseInt(document.getElementById('width').value);
    const dpi = parseInt(document.getElementById('dpi').value);
    const scaling = parseFloat(document.getElementById('scaling').value);
    const mode = document.getElementById('mode').value;
    const theme = document.getElementById('docTheme').value;
    rendererElement = document.getElementById('aplView');

    applyScale();
    performance.mark(RENDER_DOCUMENT_START);

    localStorage.setItem('aplTemplate', doc);
    localStorage.setItem('aplDataModel', data);
    localStorage.setItem('viewports', viewports);
    localStorage.setItem('height', height);
    localStorage.setItem('width', width);
    localStorage.setItem('dpi', dpi);
    localStorage.setItem('scaling', scaling);
    localStorage.setItem('mode', mode);
    localStorage.setItem('docTheme', theme);

    socket.send({
        type: 'renderDocument',
        document: doc,
        data,
        viewports
    });
};

const resizingIgnored = (width, height) => {
    const options = renderer.options;
    if (width === options.viewport.height && height === options.viewport.width) {
        // if dimensions flip, treat as rotation
        rendererElement.style.transform = `translate(-${renderer.context.getViewportWidth()}px, 0px) rotate(-90deg)`;
    } else if (width === options.viewport.width && height === options.viewport.height) {
        rendererElement.style.transform = `translate(0px, 0px) rotate(0deg)`;
    }
}

const applyScale = () => {
    const scaling = parseFloat(document.getElementById('scaling').value);
    const rendererWindow = document.getElementById('rendererWindow')
    rendererWindow.style.transform = `scale(${scaling})`;
    rendererWindow.style.transformOrigin = '0 0';
}

const rotate = () => {
    const newHeight = parseInt(document.getElementById('width').value);
    const newWidth = parseInt(document.getElementById('height').value);

    document.getElementById('width').value = newWidth;
    document.getElementById('height').value = newHeight;

    if (renderer) {
        renderer.onConfigurationChange({
            width: newWidth,
            height: newHeight
        });
    }
}

const configChange = (id, type ='string') => {
    // If we don't have a renderer, then there are no changes to apply
    if (!renderer) {
        return;
    }
    let newValue;
    switch (type) {
        case "number":
            newValue = document.getElementById(id).valueAsNumber;
            break;
        case "boolean":
            const value = document.getElementById(id).value;
            newValue = value === "true" || value === "True";
            break;
        case "object":
            const stringValue = document.getElementById(id).value;
            newValue = JSON.parse(stringValue);
            break;
        case "string":
        default:
            newValue = document.getElementById(id).value;
    }

    const configChange = {}
    configChange[id] = newValue;
    if (configChange['width']) {
        configChange['height'] = document.getElementById('height').valueAsNumber;
    }
    if (configChange['height']) {
        configChange['width'] = document.getElementById('width').valueAsNumber;
    }
    renderer.onConfigurationChange(configChange);
}

const displayStateChange = (value) => {
    // If we don't have a renderer, then there are no changes to apply
    if (!renderer) {
        return;
    }

    renderer.onDisplayStateChange({
        displayState: Number(value)
    });
}

const executeCommand = () => {
    const command = document.getElementById('aplCommand').value;
    localStorage.setItem('aplCommand', command);
    socket.send({
        type: 'executeCommands',
        command
    })
};

const attentionStateChange = (value) => {
    socket.send({
        type: 'updateAttentionSystemState',
        payload: value
    });
}

const switchTab = (evt, tabName) => {
    const content = document.getElementsByClassName('content');
    const tabs = document.getElementsByClassName('tab');
    for (const c of content) {
        c.style.display = 'none';
    }
    for (const t of tabs) {
        t.classList.remove('active');
    }
    document.getElementById(tabName).style.display = 'block';
    evt.currentTarget.classList.add('active');
};
