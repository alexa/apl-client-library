/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import APLRenderer from '../../APLRenderer';
import { VideoScale } from '../../enums/VideoScale';
import { PlaybackState } from '../../media/Resource';
import { Component, FactoryFunction } from '../Component';
import { AbstractVideoComponent } from './AbstractVideoComponent';
/**
 * @ignore
 */
export declare class VideoHolder extends AbstractVideoComponent {
    constructor(renderer: APLRenderer, component: APL.Component, factory: FactoryFunction, parent?: Component);
    onEvent(event: PlaybackState): void;
    play(waitForFinish?: boolean): Promise<void>;
    pause(): Promise<void>;
    protected setScale(scale: VideoScale): void;
}
