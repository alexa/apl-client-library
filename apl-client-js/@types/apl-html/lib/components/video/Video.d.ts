/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import APLRenderer from '../../APLRenderer';
import { AudioTrack } from '../../enums/AudioTrack';
import { CommandControlMedia } from '../../enums/CommandControlMedia';
import { VideoScale } from '../../enums/VideoScale';
import { IMediaSource } from '../../media/IMediaSource';
import { PlaybackState } from '../../media/Resource';
import { Component, FactoryFunction } from '../Component';
import { AbstractVideoComponent } from './AbstractVideoComponent';
export declare class Video extends AbstractVideoComponent {
    private readonly videoEventProcessor;
    private readonly videoEventSequencer;
    private fromEvent;
    private isSettingSource;
    constructor(renderer: APLRenderer, component: APL.Component, factory: FactoryFunction, parent?: Component);
    onEvent(event: PlaybackState): void;
    playMedia(source: IMediaSource | IMediaSource[], audioTrack: AudioTrack): Promise<void>;
    controlMedia(operation: CommandControlMedia, optionalValue: number): Promise<void>;
    play(waitForFinish?: boolean): Promise<any>;
    pause(): Promise<any>;
    end(): Promise<any>;
    seek(offset: number): Promise<any>;
    rewind(): Promise<any>;
    previous(): Promise<any>;
    next(): Promise<any>;
    setTrack(trackIndex: number): Promise<any>;
    protected setAudioTrack(audioTrack: AudioTrack): void;
    protected setMuted(muted: boolean): void;
    protected setSource(source: IMediaSource | IMediaSource[]): Promise<any>;
    protected setTrackCurrentTime(trackCurrentTime: number): void;
    protected setTrackIndex(trackIndex: number): void;
    protected setScale(scale: VideoScale): void;
    protected setTrackPaused(isPaused: boolean): void;
    protected updateMediaState(): void;
    destroy(): void;
    protected applyCssShadow: (shadowParams: string) => void;
    protected readonly player: any;
    protected readonly audioTrack: any;
    protected readonly playbackManager: any;
    protected readonly currentMediaResource: any;
    protected readonly currentMediaState: any;
}
