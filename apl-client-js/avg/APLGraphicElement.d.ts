/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { APLContext } from './../APLContext';
export declare function toActualSize(context: APLContext, value: number): number;
export declare function toFillOrStroke(context: APLContext, value: any): any;
export interface GraphicElementData {
    id: number;
    children: GraphicElementData[];
    props: {
        [key: string]: any;
    };
    type: number;
    dirtyProperties: number[];
}
export declare class APLGraphicElement implements APL.GraphicElement {
    private id;
    private type;
    private props;
    private children;
    private dirtyProperties;
    constructor(context: APLContext, data: GraphicElementData);
    getId(): number;
    getChildCount(): number;
    getChildren(): APLGraphicElement[];
    getChildAt(index: number): APL.GraphicElement;
    getValue<T>(key: number): T;
    getDirtyProperties(): number[];
    getType(): number;
    delete(): void;
}
