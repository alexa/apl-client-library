/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { ILogger } from '../../logging/ILogger';
import { AVG } from './AVG';
export interface GroupArgs {
    graphic: APL.GraphicElement;
    parent: Element;
    logger: ILogger;
    lang: string;
}
export declare class Group extends AVG {
    children: AVG[];
    constructor({ graphic, parent, logger, lang }: GroupArgs);
    private setClipPath();
    bootStrapChildren(graphic: APL.GraphicElement, logger: ILogger): void;
}
