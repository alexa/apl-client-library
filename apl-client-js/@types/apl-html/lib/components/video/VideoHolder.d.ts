/*
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
/**
 * @ignore
 */
export declare class VideoHolder extends AbstractVideoComponent {
    constructor(renderer: APLRenderer, component: APL.Component, factory: FactoryFunction, parent?: Component);
    onEvent(event: PlaybackState): void;
    playMedia(source: IMediaSource | IMediaSource[], audioTrack: AudioTrack): Promise<void>;
    controlMedia(operation: CommandControlMedia, optionalValue: number): Promise<void>;
    play(waitForFinish?: boolean): Promise<void>;
    pause(): Promise<void>;
    next(): Promise<void>;
    previous(): Promise<void>;
    rewind(): Promise<void>;
    seek(offset: number): Promise<void>;
    setTrack(trackIndex: number): Promise<void>;
    protected setScale(scale: VideoScale): void;
    protected setAudioTrack(audioTrack: AudioTrack): void;
    protected setSource(source: IMediaSource | IMediaSource[]): void;
    protected setTrackCurrentTime(trackCurrentTime: number): void;
    protected setTrackIndex(trackIndex: number): void;
}
