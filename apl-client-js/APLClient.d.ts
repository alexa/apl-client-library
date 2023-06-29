/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { APLMediaPlayerFactory, MediaPlayerFactoryFunc } from './APLMediaPlayerFactory';
import { ExtensionPayload } from './ExtensionMessageHandler';
export interface RenderingOptionsPayload {
    legacyKaraoke: boolean;
    documentAplVersion: string;
}
export interface IHierarchyPayload {
    hierarchy: IComponentPayload;
    displayedChildrenHierarchy: object;
}
export interface IComponentPayload {
    children: IComponentPayload[];
    id: string;
    type: number;
    [key: string]: any;
}
export interface MeasurePayload extends IComponentPayload {
    width: number;
    widthMode: number;
    height: number;
    heightMode: number;
}
export interface EventPayload {
    type: number;
    id: string;
    [key: string]: any;
}
export interface EventTerminatePayload {
    token: number;
}
export interface ScalingPayload {
    scaleFactor: number;
    viewportWidth: number;
    viewportHeight: number;
}
export interface BaselinePayload extends IComponentPayload {
    width: number;
    height: number;
}
export interface DocThemePayload {
    docTheme: string;
}
export interface BackgroundPayload {
    background: APL.IBackground;
}
export interface ScreenLockPayload {
    screenLock: boolean;
}
export interface IsCharacterValidPayload {
    componentId: string;
    messageId: string;
    valid: boolean;
}
export interface OnHandleKeyboardPayload {
    messageId: string;
    result: boolean;
}
export interface LocaleMethodPayload {
    method: string;
    locale: string;
    value: string;
}
export interface FocusableAreasPayload {
    messageId: string;
    areas: Map<string, APL.Rect>;
}
export interface DisplayedChildCountPayload {
    componentId: string;
    messageId: string;
    displayedChildCount: number;
}
export interface DisplayedChildIdPayload {
    componentId: string;
    messageId: string;
    displayedChildId: string;
}
export interface FocusedPayload {
    messageId: string;
    result: string;
}
export interface SupportsResizingPayload {
    supportsResizing: boolean;
}
export interface ContextPayload {
    messageId: string;
    result: string;
}
export interface AudioPlayerPayload {
    playerId: string;
}
export interface AudioPlayerSetTrackPayload extends AudioPlayerPayload {
    url: string;
}
export interface MediaPlayerPayload {
    playerId: string;
}
export interface MediaPlayerSetTrackListPayload extends MediaPlayerPayload {
    trackArray: Array<{
        url: string;
        offset: number;
        duration: number;
        repeatCount: number;
    }>;
}
export interface MediaPlayerSetTrackIndexPayload extends MediaPlayerPayload {
    index: number;
}
export interface MediaPlayerSeekPayload extends MediaPlayerPayload {
    offset: number;
}
export interface MediaPlayerSeekToPayload extends MediaPlayerPayload {
    position: number;
}
export interface MediaPlayerPlayPayload extends MediaPlayerPayload {
    waitForFinish: boolean;
}
export interface MediaPlayerSetAudioTrackPayload extends MediaPlayerPayload {
    audioTrack: number;
}
export interface MediaPlayerSetMutePayload extends MediaPlayerPayload {
    mute: boolean;
}
export interface PayloadTypeMap {
    'renderingOptions': RenderingOptionsPayload;
    'measure': MeasurePayload;
    'hierarchy': IHierarchyPayload;
    'reHierarchy': IHierarchyPayload;
    'scaling': ScalingPayload;
    'event': EventPayload;
    'dirty': IComponentPayload[];
    'eventTerminate': EventTerminatePayload;
    'baseline': BaselinePayload;
    'docTheme': DocThemePayload;
    'background': BackgroundPayload;
    'screenLock': ScreenLockPayload;
    'ensureLayout': string;
    'isCharacterValid': IsCharacterValidPayload;
    'handleKeyboard': OnHandleKeyboardPayload;
    'localeMethod': LocaleMethodPayload;
    'getFocusableAreas': FocusableAreasPayload;
    'getFocused': FocusedPayload;
    'getVisualContext': ContextPayload;
    'getDataSourceContext': ContextPayload;
    'getDisplayedChildCount': DisplayedChildCountPayload;
    'getDisplayedChildId': DisplayedChildIdPayload;
    'supportsResizing': SupportsResizingPayload;
    'extension': ExtensionPayload;
    'createAudioPlayer': AudioPlayerPayload;
    'audioPlayerPlay': AudioPlayerPayload;
    'audioPlayerSetTrack': AudioPlayerSetTrackPayload;
    'audioPlayerPause': AudioPlayerPayload;
    'audioPlayerRelease': AudioPlayerPayload;
    'mediaPlayerCreate': MediaPlayerPayload;
    'mediaPlayerDelete': MediaPlayerPayload;
    'mediaPlayerSetTrackList': MediaPlayerSetTrackListPayload;
    'mediaPlayerSetTrackIndex': MediaPlayerSetTrackIndexPayload;
    'mediaPlayerSeek': MediaPlayerSeekPayload;
    'mediaPlayerSeekTo': MediaPlayerSeekToPayload;
    'mediaPlayerPlay': MediaPlayerPlayPayload;
    'mediaPlayerPause': MediaPlayerPayload;
    'mediaPlayerStop': MediaPlayerPayload;
    'mediaPlayerNext': MediaPlayerPayload;
    'mediaPlayerPrevious': MediaPlayerPayload;
    'mediaPlayerRewind': MediaPlayerPayload;
    'mediaPlayerSetAudioTrack': MediaPlayerSetAudioTrackPayload;
    'mediaPlayerSetMute': MediaPlayerSetMutePayload;
}
export interface Message<Type extends keyof PayloadTypeMap> {
    type: Type;
    seqno: number;
    payload: PayloadTypeMap[Type];
}
export interface APLCLientEventTypeMap {
    'close': CloseEvent;
    'error': Event;
    'open': Event;
}
export interface IAPLClientListener {
    onOpen?(): void;
    onClose?(): void;
    onError?(): void;
}
export interface IAPLMessageListener {
    onMeasure?(message: Message<'measure'>): void;
    onRenderingOptions?(message: Message<'renderingOptions'>): void;
    onHierarchy?(message: Message<'hierarchy'>): void;
    onReHierarchy?(message: Message<'reHierarchy'>): void;
    onScaling?(message: Message<'scaling'>): void;
    onDirty?(message: Message<'dirty'>): void;
    onEvent?(message: Message<'event'>): void;
    onEventTerminate?(message: Message<'eventTerminate'>): void;
    onBaseline?(message: Message<'baseline'>): void;
    onBackground?(message: Message<'background'>): void;
    onScreenLock?(message: Message<'screenLock'>): void;
    onEnsureLayout?(message: Message<'ensureLayout'>): void;
    onIsCharacterValid?(message: Message<'isCharacterValid'>): void;
    onHandleKeyboard?(message: Message<'handleKeyboard'>): void;
    onLocaleMethod?(message: Message<'localeMethod'>): void;
    onGetFocusableAreas?(message: Message<'getFocusableAreas'>): void;
    onGetFocused?(message: Message<'getFocused'>): void;
    onGetVisualContext?(message: Message<'getVisualContext'>): void;
    onGetDataSourceContext?(message: Message<'getDataSourceContext'>): void;
    onGetDisplayedChildCount?(message: Message<'getDisplayedChildCount'>): void;
    onGetDisplayedChildId?(message: Message<'getDisplayedChildId'>): void;
    onSupportsResizing?(message: Message<'supportsResizing'>): void;
    onExtensionEvent?(message: Message<'extension'>): void;
    onCreateAudioPlayer?(message: Message<'createAudioPlayer'>): void;
    onAudioPlayerPlay?(message: Message<'audioPlayerPlay'>): void;
    onAudioPlayerSetTrack?(message: Message<'audioPlayerSetTrack'>): void;
    onAudioPlayerPause?(message: Message<'audioPlayerPause'>): void;
    onAudioPlayerRelease?(message: Message<'audioPlayerRelease'>): void;
    onMediaPlayerCreate?(message: Message<'mediaPlayerCreate'>): void;
    onMediaPlayerDelete?(message: Message<'mediaPlayerDelete'>): void;
    onMediaPlayerSetTrackList?(message: Message<'mediaPlayerSetTrackList'>): void;
    onMediaPlayerSetTrackIndex?(message: Message<'mediaPlayerSetTrackIndex'>): void;
    onMediaPlayerSeek?(message: Message<'mediaPlayerSeek'>): void;
    onMediaPlayerSeekTo?(message: Message<'mediaPlayerSeekTo'>): void;
    onMediaPlayerPlay?(message: Message<'mediaPlayerPlay'>): void;
    onMediaPlayerPause?(message: Message<'mediaPlayerPause'>): void;
    onMediaPlayerStop?(message: Message<'mediaPlayerStop'>): void;
    onMediaPlayerNext?(message: Message<'mediaPlayerNext'>): void;
    onMediaPlayerPrevious?(message: Message<'mediaPlayerPrevious'>): void;
    onMediaPlayerRewind?(message: Message<'mediaPlayerRewind'>): void;
    onMediaPlayerSetAudioTrack?(message: Message<'mediaPlayerSetAudioTrack'>): void;
    onMediaPlayerSetMute?(message: Message<'mediaPlayerSetMute'>): void;
}
/**
 * Extend this class to implement a client. Must implement events described in
 * `IAPLClient`
 */
export declare abstract class APLClient {
    private logger;
    private mediaPlayerFactory;
    constructor(mediaPlayerFactoryFunc?: MediaPlayerFactoryFunc);
    getMediaPlayerFactory(): APLMediaPlayerFactory;
    /**
     * Override this method to send a message
     * @param message
     */
    abstract sendMessage(message: any): any;
    /**
     * Adds a lifecycles listener
     * @param listener
     */
    addListener(listener: IAPLClientListener): void;
    /**
     * Removes a lifecycles listener
     * @param listener
     */
    removeListener(listener: IAPLClientListener): void;
    /**
     * Removes all lifecycles listeners
     */
    removeAllListeners(): void;
    /**
     * Call this when the client return isCharacterValid message with result;
     * @param message
     */
    protected isCharacterValid(message: Message<'isCharacterValid'>): void;
    /**
     * Call this when the client return getDisplayedChildCount message with result
     * @param message
     */
    protected getDisplayedChildCount(message: Message<'getDisplayedChildCount'>): void;
    /**
     * Call this when the client return getDisplayedChildId message with result
     * @param message
     */
    protected getDisplayedChildId(message: Message<'getDisplayedChildId'>): void;
    /**
     * Call this when the client return handleKeyboard message with result;
     * @param message
     */
    protected handleKeyboard(message: Message<'handleKeyboard'>): void;
    /**
     * Call this when the client return supportsResizing message with result;
     * @param message
     */
    protected supportsResizing(message: Message<'supportsResizing'>): void;
    protected extension(message: Message<'extension'>): void;
    /**
     * Call this when the client receives a message from the server
     */
    onMessage<P extends keyof PayloadTypeMap>(message: Message<P>): void;
    /**
     * Call this from  subclass when a client connection is closed
     */
    protected onClose(): void;
    /**
     * Call this from a subclass when a client connection is opened
     */
    protected onOpen(): void;
    /**
     * Call this from a subclass when there is a connection error
     */
    protected onError(): void;
}
