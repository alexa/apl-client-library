/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import APLRenderer, { IAPLOptions } from "./@types/apl-html/lib/index";
import { APLClient } from './APLClient';
import { APLComponent } from './APLComponent';
import { APLContext } from './APLContext';
export interface IAPLWSOptions extends IAPLOptions {
    scrollCommandDuration?: number;
    /**
     * The client to use to connect to the APLContent server.
     */
    client: APLClient;
    /**
     * List of extension uri's that the renderer will support.
     */
    supportedExtensions?: Array<SupportedExtension | string>;
}
/**
 * Use SupportedExtension when providing flags to an AlexaExt
 */
export interface SupportedExtension {
    uri: string;
    /**
     * If runtime-specific flags need to be provided to the extension as part of the registration
     * request, they can be one of:
     * - a single non-null value
     * - an unkeyed container (array)
     * - a key-value bag (keyed container)
     */
    flags?: string | string[] | {
        [key: string]: string;
    };
}
/**
 * The main renderer. Create a new one with `const renderer = APLWSRenderer.create(options);`
 */
export declare class APLWSRenderer extends APLRenderer<IAPLWSOptions> {
    componentMapping: {
        [id: string]: APLComponent;
    };
    private mediaPlayerFactory;
    /**
     * Creates a new renderer
     * @param options Options for this instance
     */
    static create(options: IAPLWSOptions): APLWSRenderer;
    init(): Promise<void>;
    destroy(): void;
    getContext(): APLContext;
}
