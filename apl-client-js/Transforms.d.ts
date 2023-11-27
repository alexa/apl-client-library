/*!
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { Filter } from 'apl-html';
import { APLContext } from './APLContext';
import { GraphicData } from './avg/APLGraphic';
import { GraphicPatternData } from './avg/APLGraphicPattern';
export declare function toRect(context: APLContext, value: [number, number, number, number]): APL.Rect | undefined;
export declare function toTransform(context: APLContext, value: [number, number, number, number, number, number]): string;
export declare function toColor(context: APLContext, value: any): number;
export declare function toBackground(context: APLContext, value: any): any;
export declare function toStyledText(context: APLContext, value: {
    text: string;
    spans: Array<[number, number, number, APL.SpanAttribute[]]>;
}): APL.StyledText | string;
export declare function toGraphic(context: APLContext, value: GraphicData): APL.Graphic | undefined;
export declare function toGraphicPattern(context: APLContext, value: GraphicPatternData): APL.GraphicPattern | undefined;
export declare function toRadii(context: APLContext, value: [number, number, number, number]): APL.Radii;
export declare function toDimension(context: APLContext, value: number): number;
export declare function toFilters(context: APLContext, value: Filter[]): Filter[];
export declare function toGradient(context: APLContext, value: {
    type: number;
    colorRange: string[];
    inputRange: number[];
    angle: number;
    spreadMethod: number;
    x1: number;
    y1: number;
    x2: number;
    y2: number;
    centerX: number;
    centerY: number;
    radius: number;
    units: number;
}): object;
