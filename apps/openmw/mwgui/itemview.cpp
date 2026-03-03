#include "itemview.hpp"

#include <cmath>

#include <MyGUI_FactoryManager.h>
#include <MyGUI_Gui.h>
#include <MyGUI_ImageBox.h>
#include <MyGUI_ScrollView.h>
#include <MyGUI_TextBox.h>

#include <components/settings/values.hpp>

#include "../mwbase/environment.hpp"
#include "../mwbase/inputmanager.hpp"
#include "../mwbase/windowmanager.hpp"

#include "../mwworld/class.hpp"

#include "itemmodel.hpp"
#include "itemwidget.hpp"

namespace MWGui
{

    static int getIconSize()
    {
        return Settings::gui().mControllerMenus ? 64 : 42;
    }

    static bool useListMode()
    {
        return Settings::gui().mControllerMenus;
    }

    static constexpr int ListModeCellWidth = 196;

    ItemView::ItemView()
        : mScrollView(nullptr)
        , mControllerActiveWindow(false)
    {
    }

    void ItemView::setModel(std::unique_ptr<ItemModel> model)
    {
        mModel = std::move(model);

        update();
    }

    void ItemView::initialiseOverride()
    {
        Base::initialiseOverride();

        assignWidget(mScrollView, "ScrollView");
        if (mScrollView == nullptr)
            throw std::runtime_error("Item view needs a scroll view");

        mScrollView->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
    }

    void ItemView::layoutWidgets()
    {
        if (!mScrollView->getChildCount())
            return;

        int x = 0;
        int y = 0;
        MyGUI::Widget* dragArea = mScrollView->getChildAt(0);
        int maxHeight = mScrollView->getHeight();
        mItemCount = static_cast<int>(dragArea->getChildCount());

        const int cellHeight = getIconSize();
        const int cellWidth = useListMode() ? ListModeCellWidth : cellHeight;

        mRows = std::max(maxHeight / cellHeight, 1);
        bool showScrollbar
            = static_cast<int>(std::ceil(mItemCount / float(mRows))) > mScrollView->getWidth() / cellWidth;
        if (showScrollbar)
        {
            maxHeight -= 18;
            mRows = std::max(maxHeight / cellHeight, 1);
        }

        for (int i = 0; i < mItemCount; ++i)
        {
            MyGUI::Widget* w = dragArea->getChildAt(i);

            if (useListMode())
                w->setCoord(x, y, cellWidth, cellHeight);
            else
                w->setPosition(x, y);

            y += cellHeight;

            if (y > maxHeight - cellHeight && i < mItemCount - 1)
            {
                x += cellWidth;
                y = 0;
            }
        }
        x += cellWidth;

        MyGUI::IntSize size = MyGUI::IntSize(std::max(mScrollView->getSize().width, x), mScrollView->getSize().height);

        if (Settings::gui().mControllerMenus)
        {
            if (mItemCount > 0)
                mControllerFocus = std::clamp(mControllerFocus, 0, mItemCount - 1);
            else
                mControllerFocus = -1;
            updateControllerFocus(-1, mControllerFocus);
        }

        // Canvas size must be expressed with VScroll disabled, otherwise MyGUI would expand the scroll area when the
        // scrollbar is hidden
        mScrollView->setVisibleVScroll(false);
        mScrollView->setVisibleHScroll(false);
        mScrollView->setCanvasSize(size);
        mScrollView->setVisibleVScroll(true);
        mScrollView->setVisibleHScroll(true);
        dragArea->setSize(size);
    }

    void ItemView::update()
    {
        while (mScrollView->getChildCount())
            MyGUI::Gui::getInstance().destroyWidget(mScrollView->getChildAt(0));

        if (!mModel)
            return;

        mModel->update();

        MyGUI::Widget* dragArea = mScrollView->createWidget<MyGUI::Widget>(
            {}, 0, 0, mScrollView->getWidth(), mScrollView->getHeight(), MyGUI::Align::Stretch);
        dragArea->setNeedMouseFocus(true);
        dragArea->eventMouseButtonClick += MyGUI::newDelegate(this, &ItemView::onSelectedBackground);
        dragArea->eventMouseWheel += MyGUI::newDelegate(this, &ItemView::onMouseWheelMoved);

        if (useListMode())
        {
            // List mode: each item is a cell with icon + name label
            const int iconSize = getIconSize();
            const int cellWidth = ListModeCellWidth;

            for (ItemModel::ModelIndex i = 0; i < static_cast<int>(mModel->getItemCount()); ++i)
            {
                const ItemStack& item = mModel->getItem(i);

                // Cell container
                MyGUI::Widget* cell = dragArea->createWidget<MyGUI::Widget>(
                    "", MyGUI::IntCoord(0, 0, cellWidth, iconSize), MyGUI::Align::Default);
                cell->setUserString("ToolTipType", "ItemModelIndex");
                cell->setUserData(std::make_pair(i, mModel.get()));
                cell->setNeedMouseFocus(true);
                cell->eventMouseButtonClick += MyGUI::newDelegate(this, &ItemView::onSelectedItem);
                cell->eventMouseWheel += MyGUI::newDelegate(this, &ItemView::onMouseWheelMoved);

                // Icon (first child — index 0)
                ItemWidget* itemWidget = cell->createWidget<ItemWidget>(
                    "MW_ItemIcon", MyGUI::IntCoord(0, 0, iconSize, iconSize), MyGUI::Align::Default);
                itemWidget->setNeedMouseFocus(false);
                ItemWidget::ItemState state = ItemWidget::None;
                if (item.mType == ItemStack::Type_Barter)
                    state = ItemWidget::Barter;
                if (item.mType == ItemStack::Type_Equipped)
                    state = ItemWidget::Equip;
                itemWidget->setItem(item.mBase, state);
                itemWidget->setCount(static_cast<int>(item.mCount));

                // Name label
                MyGUI::TextBox* nameLabel = cell->createWidget<MyGUI::TextBox>(
                    "SandText", MyGUI::IntCoord(iconSize + 4, 0, cellWidth - iconSize - 8, iconSize),
                    MyGUI::Align::Default);
                nameLabel->setCaption(MyGUI::UString(item.mBase.getClass().getName(item.mBase)));
                nameLabel->setTextAlign(MyGUI::Align::Left | MyGUI::Align::VCenter);
                nameLabel->setNeedMouseFocus(false);
            }
        }
        else
        {
            for (ItemModel::ModelIndex i = 0; i < static_cast<int>(mModel->getItemCount()); ++i)
            {
                const ItemStack& item = mModel->getItem(i);

                const int iconSize = getIconSize();
                ItemWidget* itemWidget = dragArea->createWidget<ItemWidget>(
                    "MW_ItemIcon", MyGUI::IntCoord(0, 0, iconSize, iconSize), MyGUI::Align::Default);
                itemWidget->setUserString("ToolTipType", "ItemModelIndex");
                itemWidget->setUserData(std::make_pair(i, mModel.get()));
                ItemWidget::ItemState state = ItemWidget::None;
                if (item.mType == ItemStack::Type_Barter)
                    state = ItemWidget::Barter;
                if (item.mType == ItemStack::Type_Equipped)
                    state = ItemWidget::Equip;
                itemWidget->setItem(item.mBase, state);
                itemWidget->setCount(static_cast<int>(item.mCount));

                itemWidget->eventMouseButtonClick += MyGUI::newDelegate(this, &ItemView::onSelectedItem);
                itemWidget->eventMouseWheel += MyGUI::newDelegate(this, &ItemView::onMouseWheelMoved);
            }
        }

        layoutWidgets();
    }

    void ItemView::resetScrollBars()
    {
        mScrollView->setViewOffset(MyGUI::IntPoint(0, 0));
        if (Settings::gui().mControllerMenus)
        {
            updateControllerFocus(mControllerFocus, 0);
            mControllerFocus = 0;
        }
    }

    void ItemView::onSelectedItem(MyGUI::Widget* sender)
    {
        ItemModel::ModelIndex index = (*sender->getUserData<std::pair<ItemModel::ModelIndex, ItemModel*>>()).first;
        eventItemClicked(index);
    }

    void ItemView::onSelectedBackground(MyGUI::Widget* /*sender*/)
    {
        eventBackgroundClicked();
    }

    void ItemView::onMouseWheelMoved(MyGUI::Widget* /*sender*/, int rel)
    {
        if (mScrollView->getViewOffset().left + rel * 0.3f > 0)
            mScrollView->setViewOffset(MyGUI::IntPoint(0, 0));
        else
            mScrollView->setViewOffset(
                MyGUI::IntPoint(static_cast<int>(mScrollView->getViewOffset().left + rel * 0.3f), 0));
    }

    void ItemView::setSize(const MyGUI::IntSize& value)
    {
        bool changed = (value.width != getWidth() || value.height != getHeight());
        Base::setSize(value);
        if (changed)
            layoutWidgets();
    }

    void ItemView::setCoord(const MyGUI::IntCoord& value)
    {
        bool changed = (value.width != getWidth() || value.height != getHeight());
        Base::setCoord(value);
        if (changed)
            layoutWidgets();
    }

    void ItemView::registerComponents()
    {
        MyGUI::FactoryManager::getInstance().registerFactory<MWGui::ItemView>("Widget");
    }

    void ItemView::setActiveControllerWindow(bool active)
    {
        mControllerActiveWindow = active;

        MWBase::Environment::get().getWindowManager()->setControllerTooltipVisible(
            active && Settings::gui().mControllerTooltips);

        if (active)
            updateControllerFocus(-1, mControllerFocus);
        else
            updateControllerFocus(mControllerFocus, -1);
    }

    void ItemView::onControllerButton(const unsigned char button)
    {
        if (!mItemCount)
            return;

        int prevFocus = mControllerFocus;
        MWBase::WindowManager* winMgr = MWBase::Environment::get().getWindowManager();

        switch (button)
        {
            case SDL_CONTROLLER_BUTTON_A:
                // Select the focused item, if any.
                if (mControllerFocus >= 0 && mControllerFocus < mItemCount)
                {
                    MyGUI::Widget* dragArea = mScrollView->getChildAt(0);
                    onSelectedItem(dragArea->getChildAt(mControllerFocus));
                }
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                // Toggle info tooltip
                winMgr->setControllerTooltipEnabled(!winMgr->getControllerTooltipEnabled());
                updateControllerFocus(-1, mControllerFocus);
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                winMgr->restoreControllerTooltips();
                if (mControllerFocus % mRows == 0)
                    mControllerFocus = std::min(mControllerFocus + mRows - 1, mItemCount - 1);
                else
                    mControllerFocus--;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                winMgr->restoreControllerTooltips();
                if (mControllerFocus % mRows == mRows - 1 || mControllerFocus == mItemCount - 1)
                    mControllerFocus -= mControllerFocus % mRows;
                else
                    mControllerFocus++;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                winMgr->restoreControllerTooltips();
                if (mControllerFocus >= mRows)
                    mControllerFocus -= mRows;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                winMgr->restoreControllerTooltips();
                if (mControllerFocus + mRows < mItemCount)
                    mControllerFocus += mRows;
                else if (mControllerFocus / mRows != (mItemCount - 1) / mRows)
                    mControllerFocus = mItemCount - 1;
                break;
            default:
                return;
        }

        if (prevFocus != mControllerFocus)
            updateControllerFocus(prevFocus, mControllerFocus);
        else
            updateControllerFocus(-1, mControllerFocus);
    }

    ItemWidget* ItemView::getItemWidget(MyGUI::Widget* child)
    {
        if (useListMode())
        {
            // In list mode, child is a row container; the ItemWidget is the first child
            if (child->getChildCount() > 0)
                return static_cast<ItemWidget*>(child->getChildAt(0));
            return nullptr;
        }
        // In grid mode, child IS the ItemWidget
        return static_cast<ItemWidget*>(child);
    }

    void ItemView::updateControllerFocus(int prevFocus, int newFocus)
    {
        MWBase::Environment::get().getWindowManager()->setCursorVisible(
            !MWBase::Environment::get().getWindowManager()->getControllerTooltipVisible());

        if (!mItemCount)
            return;

        MyGUI::Widget* dragArea = mScrollView->getChildAt(0);

        if (prevFocus >= 0 && prevFocus < mItemCount)
        {
            ItemWidget* prev = getItemWidget(dragArea->getChildAt(prevFocus));
            if (prev)
                prev->setControllerFocus(false);
        }

        if (mControllerActiveWindow && newFocus >= 0 && newFocus < mItemCount)
        {
            ItemWidget* focused = getItemWidget(dragArea->getChildAt(newFocus));
            if (focused)
            {
                focused->setControllerFocus(true);

                {
                    // Horizontal scrolling — use cellWidth for list mode, iconSize for grid mode
                    const int cellWidth = useListMode() ? ListModeCellWidth : getIconSize();
                    int column = newFocus / mRows;
                    if (column <= 3)
                        mScrollView->setViewOffset(MyGUI::IntPoint(0, 0));
                    else
                        mScrollView->setViewOffset(MyGUI::IntPoint(-cellWidth * (column - 3), 0));
                }

                MWBase::WindowManager* winMgr = MWBase::Environment::get().getWindowManager();
                winMgr->restoreControllerTooltips();

                if (winMgr->getControllerTooltipVisible())
                    MWBase::Environment::get().getInputManager()->warpMouseToWidget(focused);
            }
        }
    }
}
