"use strict";
const articles = window.BLOG_ARTICLES || [];
let category = "全部";
let visibleCount = 20;
const posts = document.querySelector("#posts");
const search = document.querySelector("#search");
const filters = document.querySelector(".filters");
const reader = document.querySelector("#reader");
const content = document.querySelector("#content");
function element(tag, text, className) {
  const node = document.createElement(tag);
  node.textContent = text;
  if (className) node.className = className;
  return node;
}
function openArticle(article) {
  content.replaceChildren(element("div", `${article.date} · ${article.category}`, "meta"), element("h2", article.title), element("div", "", "article-body"));
  content.querySelector(".article-body").innerHTML = article.html;
  const source = element("a", "在 GitHub 查看原文 ↗"); source.href = article.url; content.append(source);
  reader.showModal();
}
function render() {
  const query = search.value.trim().toLocaleLowerCase();
  const matches = articles.filter(article => (category === "全部" || category === article.category) && [article.title, article.summary, article.text].join(" ").toLocaleLowerCase().includes(query));
  posts.replaceChildren();
  for (const article of matches.slice(0, visibleCount)) {
    const button = element("button", "", "post");
    button.type = "button";
    button.append(element("div", `${article.date} · ${article.category}`, "meta"), element("h3", article.title), element("p", article.summary), element("span", "阅读文章 →", "more"));
    button.addEventListener("click", () => openArticle(article));
    posts.append(button);
  }
  if (matches.length > visibleCount) { const more = element("button", `加载更多（剩余 ${matches.length - visibleCount} 篇）`, "explore"); more.type = "button"; more.addEventListener("click", () => { visibleCount += 20; render(); }); posts.append(more); }
  if (!matches.length) posts.append(element("p", "没有找到匹配的文章，试试其他关键词。", "empty"));
  for (const button of filters.children) {
    const selected = button.textContent === category;
    button.classList.toggle("active", selected);
    button.setAttribute("aria-pressed", String(selected));
  }
}
for (const label of ["全部", ...new Set(articles.map(article => article.category))]) {
  const button = element("button", label);
  button.type = "button";
  button.addEventListener("click", () => { category = label; visibleCount = 20; render(); });
  filters.append(button);
}
search.addEventListener("input", () => { visibleCount = 20; render(); });
document.querySelector("#count").textContent = String(articles.length);
document.querySelector(".close").addEventListener("click", () => reader.close());
reader.addEventListener("click", event => {
  const bounds = reader.getBoundingClientRect();
  if (event.target === reader && (event.clientX < bounds.left || event.clientX > bounds.right || event.clientY < bounds.top || event.clientY > bounds.bottom)) reader.close();
});
document.querySelector("#all").addEventListener("click", () => { category = "全部"; search.value = ""; render(); });
document.querySelector("#aboutLink").addEventListener("click", event => { event.preventDefault(); content.replaceChildren(element("h2", "关于这个博客"), element("p", "这里收录 ZMJ 仓库中的技术文章。按目录分类，可搜索标题和正文。新增或修改 Markdown 文章并推送到 master 后，网站会自动更新。")); reader.showModal(); });
render();
