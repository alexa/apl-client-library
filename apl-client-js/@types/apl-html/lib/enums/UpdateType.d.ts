/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

export declare enum UpdateType {
    kUpdatePressed = 0,
    /**     * This component should take keyboard focus.     */
    kUpdateTakeFocus = 1,
    kUpdatePressState = 2,
    kUpdateScrollPosition = 3,
    kUpdatePagerPosition = 4,
    kUpdatePagerByEvent = 5,
    /**     * The user has pressed the submit button associated with a EditText component.     */
    kUpdateSubmit = 6,
    /**     * The user has changed the text in the edit text component.     */
    kUpdateTextChange = 7,
    /**      * Invoke an accessibility action by name.  The argument is the string name of the action to invoke.      */
    kUpdateAccessibilityAction = 8
}
