/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { ILogger } from '../logging/ILogger';
export interface MediaRequestArgs {
    onMediaLoaded?: (source: string) => void;
    onMediaLoadFailed?: (source: string, errorCode: number, error: string) => void;
    onRequestGraphic?: (source: string) => void;
    logger?: ILogger;
}
export declare const requestMedia: (mediaEvent: number, source: string[], args: MediaRequestArgs) => Promise<any>;
export declare function ensureDefaultArgs(args: MediaRequestArgs): {
    onMediaLoaded: () => void;
    onMediaLoadFailed: () => void;
    onRequestGraphic: () => void;
    logger: ILogger;
} & MediaRequestArgs;
