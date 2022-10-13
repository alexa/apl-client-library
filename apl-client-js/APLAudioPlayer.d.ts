/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { AudioPlayerFactory, IAudioEventListener, IBaseMarker } from 'apl-html';
import { APLClient, AudioPlayerSetTrackPayload } from './APLClient';
/**
 * Websocket transport for AudioPlayer. Allows controls for browser owned
 * player to be invoked through websocket and events to get back.
 */
export declare class APLAudioPlayer implements APL.AudioPlayer, IAudioEventListener {
    private client;
    private playerId;
    private player;
    private audioId;
    constructor(client: APLClient, id: string, playerFactory: AudioPlayerFactory);
    play(): void;
    release(): void;
    setTrack(payload: AudioPlayerSetTrackPayload): void;
    pause(): void;
    private doPlayerCallback(id, eventType, paused, ended, trackState);
    onPrepared(id: string): void;
    onMarker(id: string, markers: IBaseMarker[]): void;
    onPlaybackStarted(id: string): void;
    onPlaybackFinished(id: string): void;
    onError(id: string, reason: string): void;
    delete(): void;
}
