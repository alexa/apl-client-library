/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * URLRequest class stores the common elements required for any media source.
 * @ignore
 */
export interface IURLRequest {
    /**
     * The actual URL to load the media source from
     */
    url: string;
    /**
     * Request headers array
     */
    headers: string[];
}
export declare function parseHeaders(headerArray: string[]): Headers;
export declare function toUrlRequest(source: IURLRequest | string): IURLRequest;
