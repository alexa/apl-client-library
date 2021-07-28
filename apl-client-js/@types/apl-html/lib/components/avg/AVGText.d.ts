/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { GraphicPropertyKey } from '../../enums/GraphicPropertyKey';
import { AVG } from './AVG';
import { ILogger } from '../../logging/ILogger';
export interface AVGTextArgs {
    graphic: APL.GraphicElement;
    parent: Element;
    logger: ILogger;
    lang: string;
}
export declare class AVGText extends AVG {
    private textAnchors;
    constructor({ graphic, parent, logger, lang }: AVGTextArgs);
    protected setFontStyle(): (key: GraphicPropertyKey) => void;
    protected setFontWeight(): (key: GraphicPropertyKey) => void;
    protected setFontFamily(): (key: GraphicPropertyKey) => void;
    private setInnerHtml();
}
