/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { GraphicPropertyKey } from '../../enums/GraphicPropertyKey';
import { ILogger } from '../../logging/ILogger';
import { IValueWithReference } from '../Component';
export interface AVGArgs {
    graphic: APL.GraphicElement;
    parent: Element;
    logger: ILogger;
    lang?: string;
}
export declare abstract class AVG {
    /**  The svg element rendered */
    element: Element;
    graphic: APL.GraphicElement;
    protected parent: Element;
    protected logger: ILogger;
    protected lang: string;
    /** Functions to set each graphic property */
    protected graphicKeysToSetters: Map<GraphicPropertyKey, (key: GraphicPropertyKey) => void>;
    /** Elements that properties on this element require. */
    private referencedElements;
    protected constructor({ graphic, parent, logger, lang }: AVGArgs);
    setAllProperties(): void;
    updateDirty(): void;
    protected updateProperties(graphicSetters: Map<GraphicPropertyKey, (graphicPropertyKey: GraphicPropertyKey) => void>, keysToUpdate: Set<GraphicPropertyKey>): void;
    protected setAttribute(attributeName: string): (key: GraphicPropertyKey) => void;
    protected setAttributeFromMap(attributeName: string, map: Map<number, string>, defaultValue: string): (key: GraphicPropertyKey) => void;
    private setFillAndStroke(transformKey, valueKey, attributeName);
    protected setFill(): (key: GraphicPropertyKey) => void;
    protected setFillTransform(): (key: GraphicPropertyKey) => void;
    protected setStroke(): (key: GraphicPropertyKey) => void;
    protected setFontStyle(attributeName: string): (key: GraphicPropertyKey) => void;
    protected setFilter(): (key: GraphicPropertyKey) => void;
    protected createElementForAttribute(attributeName: string, createElement: () => IValueWithReference): (key: GraphicPropertyKey) => void;
}
