/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

export { AVG } from './AVG';
export { Group } from './Group';
export { Path } from './Path';
export { AVGText } from './AVGText';
export interface UpdateElementsArgs {
    root: APL.GraphicElement;
    parentElement: Element;
    dirty: {
        [key: number]: APL.GraphicElement;
    };
    lang: string;
}
export declare class VectorGraphicElementUpdater {
    private AVGByGraphicKey;
    private walkable;
    private orphanedAVGKeys;
    private readonly logger;
    constructor();
    updateElementsWithArgs({ root, parentElement, dirty, lang }: UpdateElementsArgs): void;
    /**
     * @Deprecated use updateElementsWithArgs
     */
    updateElements(root: APL.GraphicElement, parentElement: Element, dirty: {
        [key: number]: APL.GraphicElement;
    }): void;
    private walkTree(root, parentElement, dirty, lang);
    private createElement(graphicElement, parentElement, lang);
    private deleteOrphanedElements();
}
