/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { ILogger } from '../../logging/ILogger';
import { AVG } from './AVG';
export declare class Path extends AVG {
    constructor(graphic: APL.GraphicElement, parent: Element, logger: ILogger);
    private setPathLength();
}
