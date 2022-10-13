/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { AudioPlayerFactory } from 'apl-html';
import { APLAudioPlayer } from './APLAudioPlayer';
import { APLClient } from './APLClient';
/**
 * Websocket transport AudioPlayer factory.
 */
export declare class APLAudioPlayerFactory implements APL.AudioPlayerFactory {
    private client;
    private playerFactory;
    private players;
    static create(client: APLClient, playerFactory: AudioPlayerFactory): APLAudioPlayerFactory;
    constructor(client: APLClient, playerFactory: AudioPlayerFactory);
    createPlayer(id: string): APLAudioPlayer;
    getPlayer(playerId: string): APLAudioPlayer;
    tick(): void;
    delete(): void;
}
