/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { BlendMode } from '../../enums/BlendMode';
import { Filter } from '../../utils/FilterUtils';
import { IBaseFilter, IImageFilterElement } from './ImageFilter';
/**
 * @ignore
 */
export interface IBlend extends IBaseFilter {
    mode: BlendMode;
    source: number;
    destination: number;
}
export declare function getBlendFilter(filter: Filter, imageSrcArray: string[]): IImageFilterElement | undefined;
