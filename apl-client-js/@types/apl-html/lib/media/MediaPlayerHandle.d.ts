/*!
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { Video } from '../components/video/Video';
import { AudioTrack } from '../enums/AudioTrack';
import { IMediaEventListener } from './IMediaEventListener';
import { IMediaPlayerHandle } from './IMediaPlayerHandle';
import { PlaybackState } from './Resource';
export declare class MediaPlayerHandle implements IMediaPlayerHandle, IMediaEventListener {
    private readonly eventProcessor;
    private readonly eventSequencer;
    private readonly mediaPlayer;
    private videoComponent;
    private playWhenLoaded;
    private waitForFinishOnInit;
    private lastPlaybackState;
    constructor(aplMediaPlayer: APL.MediaPlayer);
    destroy(): void;
    setVideoComponent(video: Video): void;
    getProcessor(): any;
    getSequencer(): any;
    setTrackList(trackArray: Array<{
        url: string;
        offset: number;
        duration: number;
        repeatCount: number;
    }>): void;
    setTrackIndex(index: number): Promise<any>;
    seek(offset: number): Promise<any>;
    play(waitForFinish: boolean): Promise<any>;
    pause(): Promise<any>;
    stop(): Promise<any>;
    next(): Promise<any>;
    previous(): Promise<any>;
    rewind(): Promise<any>;
    setAudioTrack(audioTrack: AudioTrack): void;
    setMute(muted: boolean): void;
    onEvent(event: PlaybackState): void;
    onPlayerReady(): void;
    protected readonly player: any;
    protected readonly audioTrack: any;
    protected readonly playbackManager: any;
    protected readonly currentMediaResource: any;
    protected readonly currentMediaState: any;
}
