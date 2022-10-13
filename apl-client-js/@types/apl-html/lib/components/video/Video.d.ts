/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import APLRenderer from '../../APLRenderer';
import { VideoScale } from '../../enums/VideoScale';
import { PlaybackState } from '../../media/Resource';
import { Component, FactoryFunction } from '../Component';
import { AbstractVideoComponent } from './AbstractVideoComponent';
export declare class Video extends AbstractVideoComponent {
    private readonly videoEventProcessor;
    private mediaPlayerHandle;
    constructor(renderer: APLRenderer, component: APL.Component, factory: FactoryFunction, parent?: Component);
    protected applyCssShadow: (shadowParams: string) => void;
    protected setScale(scale: VideoScale): void;
    destroy(): void;
    play(waitForFinish?: boolean): Promise<void>;
    pause(): Promise<void>;
    onEvent(_event: PlaybackState): void;
    protected readonly player: any;
    protected readonly audioTrack: any;
    protected readonly playbackManager: any;
    protected readonly currentMediaResource: any;
    protected readonly currentMediaState: any;
}
