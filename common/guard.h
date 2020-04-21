/**
 * @file guard.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief   generic scope guard
 * @version 0.1
 * @date 2020-04-21
 *
 * @copyright Copyright (c) 2020
 *
 */

template <typename Function>
class Guard final {
   public:
    explicit Guard(Function&& f) : func_{std::forward<Function>(f)} {}
    ~Guard() {
        func_();
    }

    Guard(Guard<Function> const& rhs) = delete;
    auto operator=(Guard<Function> const& rhs) -> Guard& = delete;
    Guard(Guard<Function>&& rhs) = delete;
    auto operator=(Guard<Function>&& rhs) -> Guard& = delete;

   private:
    Function func_;
};