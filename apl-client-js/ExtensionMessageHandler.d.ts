/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { APLContext } from './APLContext';
export declare enum ExtensionMessageTypes {
    Event = "event"
}
export interface ExtensionPayload {
    type: ExtensionMessageTypes;
    URI: string;
    name: string;
    source: any;
    params: any;
}
export declare class ExtensionMessageHandler {
    private extensions;
    private readonly context;
    constructor(context: APLContext);
    private getOrCreate(uri);
    handleExtensionEvent(payload: ExtensionPayload): any;
}
