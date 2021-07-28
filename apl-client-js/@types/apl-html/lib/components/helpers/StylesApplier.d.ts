/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * Adjust exports to only export used interfaces
 */
export interface StylesApplier {
    applyStyle(): void;
}
export interface CssAttributeValues {
    [key: string]: string | number;
}
export declare type CssUnitTypes = 'px' | 'em' | '%' | '';
export declare type ElementTypes = 'HTML' | 'SVG';
export interface SupportedElementTypes {
    [key: string]: ElementTypes;
}
export declare const ElementType: SupportedElementTypes;
export interface StylesApplierArgs {
    element: Element;
    properties: CssAttributeValues;
    cssUnitType?: CssUnitTypes;
    elementType?: ElementTypes;
}
export interface SupportedCSSUnitTypes {
    [key: string]: CssUnitTypes;
}
export declare const CssUnitType: SupportedCSSUnitTypes;
export declare function createStylesApplier(args: StylesApplierArgs): StylesApplier;
